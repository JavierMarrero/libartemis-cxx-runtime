/*
 * Copyright (C) 2022 Javier Marrero.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

/* 
 * File:   HashMap.h
 * Author: Javier Marrero
 *
 * Created on December 6, 2022, 6:26 PM
 */

#ifndef HASHMAP_H
#define HASHMAP_H

// API
#include <Axf/Collections/Bits/HashUtils.h>
#include <Axf/Collections/Map.h>
#include <Axf/Collections/HashSet.h>
#include <Axf/Core/Lang-C++/reference-wrapper.h>

// C++
#include <cstring>

namespace axf
{
namespace collections
{
namespace __
{

/**
 * Represents the hash map entries.
 *
 * @return
 */
template <typename K, typename V>
class HashMapEntry : public Entry<K, V>
{

    AXF_CLASS_TYPE(AXF_TEMPLATE_CLASS(axf::collections::__::HashMapEntry<K, V>),
                   AXF_TYPE(AXF_TEMPLATE_CLASS(Entry<K, V>)))
public:

    HashMapEntry(const K& key, const K& value)
    : Entry<K, V>(key, value), m_next(NULL) { }

    ~HashMapEntry()
    {
        m_next = NULL;
    }

    inline void setNext(HashMapEntry<K, V>* next)
    {
        m_next = next;
    }

    inline HashMapEntry<K, V>* getNext() const
    {
        return m_next;
    }

private:

    HashMapEntry<K, V>* m_next;
} ;

}

/**
 * In computer sciences, a <i>hash table</i>, also known as a <i>hash map</i>
 * is a data structure that implements associative arrays or dictionary.
 * <p>
 * A hash table uses a hash function to compute an index, also called the hash
 * code into an array of buckets or slots, from which the desired value can be
 * found. During look-up, the key is hashed and the correspondent value indicates
 * where the value is stored.
 * <p>
 * Ideally, the hash function will evenly distribute keys amongst all the
 * available buckets, but more often than not, it is used an imperfect hash
 * function in order to not to affect performance. Collisions are handled using
 * a <i>chained hash</i> approach.
 * <p>
 * In a well-dimensioned hash table, the cost of insertion, look-up and removal
 * is independent of the number of elements. This particular implementation
 * provides amortized constant time operations.
 *
 * @author J. Marrero
 */
template
<
typename K,
typename V,
typename hash_functor_t = __::default_hash<K>,
typename array_allocator_t = axf::collections::DefaultAllocator<__::HashMapEntry<K, V>*>,
typename entry_allocator_t = axf::collections::DefaultAllocator<__::HashMapEntry<K, V> >
>
class HashMap : public Map<K, V>
{
    AXF_CLASS_TYPE(AXF_TEMPLATE_CLASS(axf::collections::HashMap<K, V, hash_functor_t, array_allocator_t, entry_allocator_t>),
                   AXF_TYPE(AXF_TEMPLATE_CLASS(Map<K, V>)))

private:

    typedef __::HashMapEntry<K, V> entry_t;
    typedef HashSet<core::reference_wrapper<const K> > keyset_t;
    typedef HashSet<core::reference_wrapper<const V> > valset_t;

public:

    static const std::size_t    DEFAULT_CAPACITY = 16;
    static const float          DEFAULT_LOAD_FACTOR = 0.75f;

    HashMap(std::size_t initialSize = DEFAULT_CAPACITY, float loadFactor = DEFAULT_LOAD_FACTOR)
    :
    m_buckets(NULL),
    m_capacity(initialSize),
    m_keySet(initialSize, loadFactor),
    m_loadFactor(loadFactor),
    m_modCount(0),
    m_size(0),
    m_threshold((std::size_t) (initialSize * loadFactor)),
    m_valueSet(initialSize, loadFactor)
    {
        // Allocate the new array
        m_buckets = m_bucketAllocator.newArray(initialSize);
    }

    ~HashMap()
    {
        // Iterate through all the buckets
        for (std::size_t i = 0; i < m_capacity; ++i)
        {
            entry_t* current = m_buckets[i];
            while (current != NULL)
            {
                // Store the old entry
                entry_t* next = current->getNext();

                this->deleteEntry(current);

                // Jump
                current = next;
            }
        }

        // Release
        m_bucketAllocator.deleteArray(m_buckets, m_capacity);

        // Set the fields to invalid values
        m_capacity = 0;
        m_loadFactor = 0.0f;
        m_size = 0;
        m_threshold = 0;
    }

    virtual iterator<Entry<K, V> > begin()
    {
        return NULL;
    }

    virtual const iterator<Entry<K, V> > begin() const
    {
        return NULL;
    }

    virtual bool contains(const Entry<K, V>& element) const
    {
        std::size_t index = calculateHash(element.getKey());
        entry_t* entry = m_buckets[index];
        while (entry != NULL)
        {
            if (entry->getKey() == element.getKey() && entry->getValue() == element.getValue())
            {
                return true;
            }
        }

        return false;
    }

    virtual iterator<Entry<K, V> > end()
    {
        return NULL;
    }

    virtual const iterator<Entry<K, V> > end() const
    {
        return NULL;
    }

    inline virtual bool isEmpty() const
    {
        return m_size == 0;
    }

    virtual keyset_t& keySet()
    {
        return m_keySet;
    }

    virtual bool put(const K& key, const V& value)
    {
        return add(Entry<K, V>(key, value));
    }

    inline virtual size_t size() const
    {
        return m_size;
    }

    virtual valset_t& valueSet()
    {
        return m_valueSet;
    }

protected:

    virtual bool add(const Entry<K, V>& element)
    {
        if (m_size + 1 > m_threshold)
            rehash();

        entry_t* entry = allocateEntry(element.getKey(), element.getValue());
        bool result = (entry != NULL);
        if (result)
        {
            std::size_t index = calculateHash(entry->getKey());
            if (m_buckets[index] == NULL)
            {
                m_buckets[index] = entry;
            }
            else
            {
                entry_t* dest = m_buckets[index];
                if (dest->getKey() == element.getKey())
                    result = false;

                while (dest->getNext() != NULL && result)
                {
                    if (dest->getKey() == entry->getKey())
                        result = false;
                    dest = dest->getNext();
                }

                dest->setNext(entry);
            }

            if (result)
            {
                m_size++;

                // Add to the key and value sets


                // Mod-count
                m_modCount++;
            }
            else
            {
                // Clean-up
                deleteEntry(entry);
            }
        }
        return result;
    }

    virtual bool remove(const Entry<K, V>& element)
    {
        return false;
    }

private:

    entry_t**           m_buckets;
    array_allocator_t   m_bucketAllocator;
    std::size_t         m_capacity;
    entry_allocator_t   m_entryAllocator;
    hash_functor_t      m_hasher;
    keyset_t            m_keySet;
    float               m_loadFactor;
    long                m_modCount;
    std::size_t         m_size;
    std::size_t         m_threshold;
    valset_t            m_valueSet;

    inline entry_t* allocateEntry(const K& key, const V& value)
    {
        return m_entryAllocator.newObject(entry_t(key, value));
    }

    inline std::size_t calculateHash(const K& key) const
    {
        return calculateHash(key, m_capacity);
    }

    inline std::size_t calculateHash(const K& key, const std::size_t limit) const
    {
        return m_hasher(key) % limit;
    }

    inline void deleteEntry(entry_t* entry)
    {
        m_entryAllocator.deleteObject(entry);
    }

    void rehash()
    {
        entry_t** oldBuckets = m_buckets;
        std::size_t oldCapacity = m_capacity;

        std::size_t newCapacity = (m_capacity * 2) + 1;
        entry_t** newBuckets = m_bucketAllocator.newArray(newCapacity);

        for (std::size_t i = 0; i < oldCapacity; ++i)
        {
            entry_t* current = oldBuckets[i];
            while (current != NULL)
            {
                // Store the next element
                entry_t* next = current->getNext();

                // Rehash
                std::size_t index = calculateHash(current->getKey(), newCapacity);
                entry_t* dest = newBuckets[index];
                newBuckets[index] = current;
                current->setNext(dest);

                // Iterate
                current = next;
            }
        }

        // Reassign
        m_buckets = newBuckets;
        m_capacity = newCapacity;

        // Deallocate
        m_bucketAllocator.deleteArray(oldBuckets, oldCapacity);

        // Recalculate threshold
        m_threshold = (std::size_t) (newCapacity * m_loadFactor);
    }
} ;

}
}

#endif /* HASHMAP_H */

