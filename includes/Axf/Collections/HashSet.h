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
 * File:   HashSet.h
 * Author: Javier Marrero
 *
 * Created on December 7, 2022, 3:36 PM
 */

#ifndef HASHSET_H
#define HASHSET_H

// API
#include <Axf/Collections/ConcurrentModificationException.h>
#include <Axf/Collections/Set.h>
#include <Axf/Collections/ArrayList.h>
#include <Axf/Core/Array.h>
#include <Axf/Core/IllegalArgumentException.h>
#include <Axf/Core/IllegalStateException.h>

namespace axf
{
namespace collections
{
namespace __
{

/**
 * Represents an entry in the hash-set.
 *
 * @return
 */
template <typename T>
struct HashSetEntry : public core::Object
{

    AXF_CLASS_TYPE(axf::collections::__::HashSetEntry<T>,
                   AXF_TYPE(axf::core::Object))
public:

    HashSetEntry(const T& data) : m_data(data), m_next(NULL) { }

    ~HashSetEntry()
    {
        m_next = NULL;
    }

    T                   m_data;
    HashSetEntry<T>*    m_next;
} ;

/**
 * Represents an iterator for the hash set class.
 *
 * @return
 */
template <typename T>
class HashSetIterator : public BasicIterator<T>
{
    AXF_CLASS_TYPE(axf::collections::__::HashSetIterator<T>,
                   AXF_TYPE(axf::collections::BasicIterator<T>))

    typedef HashSetEntry<T> entry_t;

public:

    /**
     * A sentinel value signaling the end of this collection.
     */
    static const std::size_t NPOS = static_cast<std::size_t> (-1);

    HashSetIterator(entry_t* * const* buckets_ptr, std::size_t capacity, const std::size_t count)
    :
    m_buckets(buckets_ptr), m_capacity(capacity), m_count(count), m_current(NULL), m_index(0)
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

    ~HashSetIterator()
    {
        m_count = NPOS;
        m_current = NULL;
        m_index = NPOS;
    }

    virtual T& current()
    {
        return m_current->m_data;
    }

    virtual const T& current() const
    {
        return m_current->m_data;
    }

    virtual bool equals(const core::Object& object) const
    {
        const HashSetIterator<T>& rhs = static_cast<const HashSetIterator<T>&> (object);

        return m_count == rhs.m_count;
    }

    virtual T& next()
    {
        T& result = current();
        forward();
        return result;
    }

    virtual const T& next() const
    {
        const T& result = current();
        forward();
        return result;
    }

private:

    entry_t* * const*   m_buckets;
    std::size_t         m_capacity;
    mutable std::size_t m_count;
    mutable entry_t*    m_current;
    mutable std::size_t m_index;

    inline void forward() const
    {
        if (m_count == NPOS)
            throw core::IllegalStateException("attempted to advance an end-of-range iterator.");

        if (m_current->m_next != NULL)
        {
            m_current = m_current->m_next;
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
 * A hash set is a non sorted set that uses hashing to store the reference to
 * their elements.
 * <p>
 * Hash sets provided amortized constant time (<i>O(1)</i>) for almost every
 * operation.
 *
 * @return
 */
template
<
typename E,
typename hasher_t = axf::collections::__::default_hash<E>,
typename array_alloc_t = axf::collections::DefaultAllocator<__::HashSetEntry<E>*>,
typename entry_alloc_t = axf::collections::DefaultAllocator<__::HashSetEntry<E> >
>
class HashSet : public Set<E>
{
    AXF_CLASS_TYPE(
                   AXF_TEMPLATE_CLASS(axf::collections::HashSet<E, array_alloc_t, entry_alloc_t>),
                   AXF_TYPE(axf::collections::Set<E>))

    template <typename>
    friend class __::HashSetIterator;

    typedef __::HashSetEntry<E>     entry_t;
    typedef __::HashSetIterator<E>  iterator_t;

    static const int DEFAULT_CAPACITY = 16;
    static const float DEFAULT_LOAD_FACTOR = 0.75f;

public:

    HashSet(std::size_t initialCapacity = DEFAULT_CAPACITY, float loadFactor = DEFAULT_LOAD_FACTOR)
    :
    m_buckets(NULL),
    m_capacity(initialCapacity),
    m_loadFactor(loadFactor),
    m_modCount(0),
    m_size(0),
    m_threshold((long) (initialCapacity * loadFactor))
    {
        if (!(loadFactor > 0.0f))
        {
            throw core::IllegalArgumentException("illegal load factor: %f", loadFactor);
        }

        if (initialCapacity == 0)
            m_capacity = 1;

        // Allocate the array
        m_buckets = m_buckets_allocator.newArray(m_capacity);
    }

    ~HashSet()
    {
        // Deallocate all the entries
        for (std::size_t i = 0; i < m_capacity; i++)
        {
            entry_t* current = m_buckets[i];
            while (current != NULL)
            {
                entry_t* next = current->m_next;

                // Delete the entry
                deleteEntry(current);

                current = next;
            }
        }

        // Deallocate the array
        m_buckets_allocator.deleteArray(m_buckets, m_capacity);

        // Set the fields
        m_capacity = 0;
        m_loadFactor = 0.0f;
        m_modCount = 0;
        m_size = 0;
        m_threshold = 0;
    }

    virtual bool add(const E& element)
    {
        // Check if it is necessary to rehash
        if (m_size + 1 > m_threshold)
        {
            rehash();
        }

        // Prepare a new entry for insertion
        entry_t* newEntry = allocateEntry(element);
        bool result = (newEntry != NULL);
        if (result)
        {
            std::size_t index = calculateHash(element, m_capacity);
            entry_t* current = m_buckets[index];

            if (current == NULL)
            {
                m_buckets[index] = newEntry;
            }
            else
            {
                if (current->m_data == element)
                    result = false;

                while (current->m_next != NULL && result)
                {
                    if (current->m_data == element)
                    {
                        result = false;
                    }
                    current = current->m_next;
                }

                if (result)
                    current->m_next = newEntry;
            }

            if (result == false)
            {
                deleteEntry(newEntry);
            }
            else
            {
                m_size++;
            }
        }
        return result;
    }

    virtual iterator<E> begin()
    {
        return new iterator_t(&m_buckets, m_capacity, m_size);
    }

    virtual const iterator<E> begin() const
    {
        return new iterator_t(&m_buckets, m_capacity, m_size);
    }

    virtual bool contains(const E& element) const
    {
        std::size_t index = calculateHash(element, m_capacity);
        if (m_buckets[index] != NULL)
        {
            entry_t* current = m_buckets[index];
            while (current != NULL)
            {
                if (current->m_data == element)
                    return true;
                current = current->m_next;
            }
        }
        return false;
    }

    virtual const iterator<E> end() const
    {
        return new iterator_t(&m_buckets, m_capacity, iterator_t::NPOS);
    }

    virtual iterator<E> end()
    {
        return new iterator_t(&m_buckets, m_capacity, iterator_t::NPOS);
    }

    virtual bool isEmpty() const
    {
        return m_size == 0;
    }

    virtual bool remove(const E& element)
    {
        bool result = false;
        std::size_t index = calculateHash(element, m_capacity);

        if (m_buckets[index] != NULL)
        {
            entry_t* current = m_buckets[index];
            if (current->m_data == element)
            {
                m_buckets[index] = current->m_next;
                deleteEntry(current);
                result = true;
            }
            else
            {
                entry_t* prev = m_buckets[index];
                current = prev->m_next;

                while (current != NULL && result == false)
                {
                    if (current->m_data == element)
                    {
                        prev->m_next = current->m_next;
                        deleteEntry(current);
                        result = true;
                    }
                }
            }
        }

        if (result)
        {
            m_size--;
        }
        return result;
    }

    inline size_t size() const
    {
        return m_size;
    }

private:

    entry_t**       m_buckets;
    array_alloc_t   m_buckets_allocator;
    std::size_t     m_capacity;
    entry_alloc_t   m_entry_allocator;
    hasher_t        m_hasher;
    float           m_loadFactor;
    int             m_modCount;
    std::size_t     m_size;
    unsigned long   m_threshold;

    inline entry_t* allocateEntry(const E& key)
    {
        return m_entry_allocator.newObject(key);
    }

    inline std::size_t calculateHash(const E& key, std::size_t limit) const
    {
        return m_hasher(key) % limit;
    }

    inline void deleteEntry(entry_t* rhs)
    {
        return m_entry_allocator.deleteObject(rhs);
    }

    /**
     * Increases the size of the HashMap and rehashes all keys to new
     * array indices; this is called when the addition of a new value
     * would cause size() &gt; threshold. Note that the existing Entry
     * objects are reused in the new hash set.
     * <p>
     * This is not specified, but the new size is twice the current size
     * plus one; this number is not always prime, unfortunately.
     */
    void rehash()
    {
        entry_t** oldBuckets = m_buckets;
        std::size_t oldCapacity = m_capacity;

        std::size_t newCapacity = (m_capacity * 2) + 1;
        entry_t** newBuckets = m_buckets_allocator.newArray(newCapacity);

        for (std::size_t i = 0; i < oldCapacity; ++i)
        {
            entry_t* current = oldBuckets[i];
            while (current != NULL)
            {
                // Store the next element
                entry_t* next = current->m_next;

                // Rehash
                std::size_t index = calculateHash(current->m_data, newCapacity);
                entry_t* dest = newBuckets[index];
                newBuckets[index] = current;
                current->m_next = dest;

                // Iterate
                current = next;
            }
        }

        // Reassign
        m_buckets = newBuckets;
        m_capacity = newCapacity;

        // Deallocate
        m_buckets_allocator.deleteArray(oldBuckets, oldCapacity);

        // Recalculate threshold
        m_threshold = (long) (newCapacity * m_loadFactor);
    }
} ;

}
}

#endif /* HASHSET_H */

