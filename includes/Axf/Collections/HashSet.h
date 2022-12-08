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
#include <Axf/Collections/Set.h>

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
class HashSetEntry : public core::Object
{

    AXF_CLASS_TYPE(axf::collections::__::HashSetEntry<T>,
                   AXF_TYPE(axf::core::Object))
public:

    HashSetEntry(const T& data) : m_data(data), m_next(NULL) { }

    ~HashSetEntry() { }

    inline const T& getData() const
    {
        return m_data;
    }

    inline void setData(const T& data)
    {
        m_data = data;
    }

    inline void setNext(HashSetEntry<T>* next)
    {
        m_next = next;
    }

    inline HashSetEntry<T>* getNext() const
    {
        return m_next;
    }

private:

    T                   m_data;
    HashSetEntry<T>*    m_next;
} ;

}

/**
 * A hash set is a non sorted set that uses hashing to store the reference to
 * their elements.
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

public:

    HashSet(std::size_t initialCapacity = 64) : m_array(NULL), m_arraySize(0), m_size(0)
    {
        resizeArray(initialCapacity);
    }

    ~HashSet()
    {
        m_arrayAllocator.deleteObject(m_array);
    }

    bool add(const E& element)
    {
        unsigned int index = hashKey(element);
        __::HashSetEntry<E>* entry = m_entryAllocator.newObject(element);
        bool result = entry != NULL;

        if (entry)
        {
            if (m_array[index] == NULL)
            {
                m_array[index] = entry;
            }
            else
            {
                __::HashSetEntry<E>* current = m_array[index];
                if (current->getData() == element)
                {
                    result = false;
                }

                while (current->getNext() != NULL && result)
                {
                    current = current->getNext();
                    if (current->getData() == element)
                    {
                        result = false;
                    }
                }

                current->setNext(entry);
            }
        }

        if (result)
        {
            m_size++;
        }
        return result;
    }

    iterator_ref<E> begin()
    {
        return NULL;
    }

    virtual const iterator_ref<E> begin() const
    {
        return NULL;
    }

    virtual bool contains(const E& element) const
    {
        bool result = false;
        int idx = hashKey(element);

        if (m_array[idx] != NULL)
        {
            __::HashSetEntry<E>* current = m_array[idx];
            while (current != NULL && !result)
            {
                if (current->getData() == element)
                    result = true;
                current = current->getNext();
            }
        }
        return result;
    }

    iterator_ref<E> end()
    {
        return NULL;
    }

    virtual const iterator_ref<E> end() const
    {
        return NULL;
    }

    bool isEmpty() const
    {
        return m_size == 0;
    }

    bool remove(const E& element)
    {
        return false;
    }

    size_t size() const
    {
        return m_size;
    }

private:

    __::HashSetEntry<E>**   m_array;
    array_alloc_t           m_arrayAllocator;
    std::size_t             m_arraySize;
    entry_alloc_t           m_entryAllocator;
    hasher_t                m_hasher;
    std::size_t             m_size;

    inline unsigned int hashKey(const E& key) const
    {
        return m_hasher(key) % m_arraySize;
    }

    __::HashSetEntry<E>** resizeArray(const std::size_t newCapacity)
    {
        if (m_array == NULL)
        {
            // The array is new
            m_array = m_arrayAllocator.allocate(newCapacity);
            m_arraySize = newCapacity;
        }
        else
        {

        }
        return m_array;
    }

} ;

}
}

#endif /* HASHSET_H */

