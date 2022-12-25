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
#include <Axf/Collections/ArrayList.h>
#include <Axf/Collections/HashSet.h>
#include <Axf/Collections/NoSuchElementException.h>
#include <Axf/Core/Lang-C++/reference-wrapper.h>
#include <Axf/Core/IllegalStateException.h>

// C++
#include <cstring>
#include <cstdio>

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

    HashMapEntry(const K& key, const V& value)
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

template <typename K, typename V>
class HashMapIterator : public BasicIterator<Entry<K, V> >
{
    AXF_CLASS_TYPE(AXF_TEMPLATE_CLASS(axf::collections::__::HashMapIterator<K, V>),
                   AXF_TYPE(AXF_TEMPLATE_CLASS(BasicIterator<Entry<K, V> >)))

    typedef HashMapEntry<K, V> entry_t;

public:

    static const std::size_t NPOS = static_cast<std::size_t> (-1);

    HashMapIterator(entry_t* * const* buckets, const std::size_t capacity, std::size_t count)
    :
    m_buckets(buckets), m_capacity(capacity), m_current(NULL), m_count(count), m_index(0)
    {
        if (count != NPOS)
        {
            for (; m_index < m_capacity && (m_current == NULL); ++m_index)
            {
                if (bucketAt(m_index) != NULL)
                {
                    m_current = bucketAt(m_index);
                    m_count--;
                }
            }
        }
    }

    virtual Entry<K, V>& current()
    {
        return *m_current;
    }

    virtual const Entry<K, V>& current() const
    {
        return *m_current;
    }

    virtual bool equals(const core::Object& object) const
    {
        const HashMapIterator<K, V>& rhs = static_cast<const HashMapIterator<K, V>&> (object);

        return m_count == rhs.m_count;
    }

    virtual Entry<K, V>& next()
    {
        Entry<K, V>& result = current();
        forward();
        return result;
    }

    virtual const Entry<K, V>& next() const
    {
        const Entry<K, V>& result = current();
        forward();
        return result;
    }

private:

    entry_t* * const*   m_buckets;
    const std::size_t   m_capacity;
    mutable entry_t*    m_current;
    mutable std::size_t m_count;
    mutable std::size_t m_index;

    inline void forward() const
    {
        if (m_count == NPOS)
            throw core::IllegalStateException("attempted to advance an end-of-range iterator.");

        if (m_current->getNext() != NULL)
        {
            m_current = m_current->getNext();
        }
        else
        {
            m_current = NULL;
            for (; m_index < m_capacity && (m_current == NULL); ++m_index)
            {
                if (bucketAt(m_index) != NULL)
                {
                    m_current = bucketAt(m_index);
                }
            }
        }

        if (m_count == 0)
        {
            m_count = NPOS;
        }
        else
        {
            m_count--;
        }
    }

    inline entry_t* bucketAt(std::size_t index) const
    {
        return (*m_buckets)[index];
    }
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

    struct keyset_hasher_t
    {
        __::default_hash<K> m_hasher;

        unsigned int operator()(const core::reference_wrapper<const K>& wrappah) const
        {
            return m_hasher(wrappah.get());
        }
    } ;

    typedef __::HashMapEntry<K, V> entry_t;
    typedef __::HashMapIterator<K, V> iterator_t;
    typedef HashSet<core::reference_wrapper<const K>, keyset_hasher_t>  keyset_t;
    typedef ArrayList<core::reference_wrapper<V> >                      valset_t;

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
    m_valueSet(initialSize)
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
        if (isEmpty()) return end();

        return new iterator_t(&m_buckets, m_capacity, m_size);
    }

    virtual const iterator<Entry<K, V> > begin() const
    {
        if (isEmpty()) return end();

        return new iterator_t(&m_buckets, m_capacity, m_size);
    }

    virtual void clear()
    {
        // Iterate and delete all the buckets
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

            m_buckets[i] = NULL;
        }

        // Clear both sets
        m_keySet.clear();
        m_valueSet.clear();

        // Parameters
        m_size = 0;
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
        return new iterator_t(&m_buckets, m_capacity, iterator_t::NPOS);
    }

    virtual const iterator<Entry<K, V> > end() const
    {
        return new iterator_t(&m_buckets, m_capacity, iterator_t::NPOS);
    }

    virtual V& get(const K& key) const
    {
        std::size_t index = calculateHash(key);
        if (m_buckets[index] != NULL)
        {
            entry_t* current = m_buckets[index];
            while (current != NULL)
            {
                if (current->getKey() == key)
                {
                    return current->getValue();
                }
                current = current->getNext();
            }
        }

        char msgBuffer[128] = {0};
        std::sprintf(msgBuffer, "no element with index %lu is present on the map.", (unsigned long) index);

        throw NoSuchElementException(msgBuffer);
    }

    inline virtual bool isEmpty() const
    {
        return m_size == 0;
    }

    virtual inline const keyset_t& keySet() const
    {
        return m_keySet;
    }

    virtual bool put(const K& key, const V& value)
    {
        return add(Entry<K, V>(key, value));
    }

    virtual bool remove(const K& key)
    {
        bool result = false;
        std::size_t index = calculateHash(key, m_capacity);

        if (m_buckets[index] != NULL)
        {
            entry_t* current = m_buckets[index];
            if (current->getKey() == key)
            {
                m_buckets[index] = current->getNext();
                deleteEntry(current);
                result = true;
            }
            else
            {
                entry_t* prev = m_buckets[index];
                current = prev->getNext();

                while (current != NULL && result == false)
                {
                    if (current->getKey() == key)
                    {
                        m_valueSet.remove(current->getValue());

                        prev->setNext(current->getNext());
                        deleteEntry(current);
                        result = true;
                    }
                }
            }
        }

        if (result)
        {
            m_size--;

            // Delete the key reference reference
            m_keySet.remove(key);
        }
        return result;
    }

    inline virtual size_t size() const
    {
        return m_size;
    }

    virtual inline const valset_t& values() const
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
                m_keySet.add(core::ref(entry->getKey()));
                m_valueSet.add(core::ref(entry->getValue()));

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
        return remove(element.getKey());
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

