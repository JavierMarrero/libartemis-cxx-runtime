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

// C++
#include <cstring>

namespace axf
{
namespace collections
{

template
<
typename K,
typename V,
typename hash_functor_t = __::default_hash<K>,
typename array_allocator_t = axf::collections::DefaultAllocator<Entry<K, V>*>,
typename entry_allocator_t = axf::collections::DefaultAllocator<Entry<K, V> >,
typename keyset_allocator_t = axf::collections::DefaultAllocator<HashSet<K*> >,
typename valset_allocator_t = axf::collections::DefaultAllocator<HashSet<V*> >
>
class HashMap : public Map<K, V>
{

    AXF_CLASS_TYPE(AXF_TEMPLATE_CLASS(axf::collections::HashMap<K, V, hash_functor_t, array_allocator_t, entry_allocator_t, keyset_allocator_t, valset_allocator_t>),
                   AXF_TYPE(AXF_TEMPLATE_CLASS(Map<K, V>)))

public:

    HashMap(unsigned initialSize = 64)
    :
    m_array(NULL), m_arraySize(initialSize)
    {
        resizeArray(m_arraySize);
    }

    ~HashMap()
    {
        m_arrayAllocator.deleteObject(m_array);
    }

    bool add(const Entry<K, V>& element)
    {
        Entry<K, V>* entry = allocateEntry(element.getKey(), element.getValue());

        bool result = (entry == NULL);
        if (entry)
        {
            unsigned int idx = hashKey(element.getKey());
            if (m_array[idx] == NULL)
            {
                m_array[idx] = entry;
                result = true;
            }
            else
            {
                // Check if matches
                Entry<K, V>* atIndex = m_array[idx];
                result = element.getKey() != atIndex->getKey();
                if (result)
                {
                    Entry<K, V>* current = atIndex;
                    while (current->getNext() != NULL && result == true)
                    {
                        result = current->getKey() == element.getKey();
                        current = current->getNext();
                    }

                    // Able to insert
                    if (result)
                    {
                        current->setNext(entry);
                    }
                }
            }
        }

        if (result)
            m_size++;
        return result;
    }

    inline iterator_ref<Entry<K, V> > begin()
    {
        return NULL;
    }

    virtual const iterator_ref<Entry<K, V> > begin() const
    {
        return NULL;
    }

    virtual bool contains(const Entry<K, V>& element) const
    {
        bool result = false;
        for (const iterator_ref<Entry<K, V> > it = begin(),
             end = this->end(); it != end && result == false; it->next())
        {
            const Entry<K, V>& current = it->current();
            if (current.getKey() == element.getKey() &&
                current.getValue() == element.getValue())
            {
                result = true;
            }
        }
        return result;
    }

    inline iterator_ref<Entry<K, V> > end()
    {
        return NULL;
    }

    virtual const iterator_ref<Entry<K, V> > end() const
    {
        return NULL;
    }

    inline bool isEmpty() const
    {
        return m_size == 0;
    }

    virtual const Set<const K*>& keySet()
    {
        return m_keySet;
    }

    inline size_t size() const
    {
        return m_size;
    }

    bool put(const K& key, const V& value)
    {
        return add(Entry<K, V>(key, value));
    }

    bool remove(const Entry<K, V>& element)
    {
        return false;
    }

    virtual const Set<const V*>& valueSet()
    {
        return m_valueSet;
    }

private:

    Entry<K, V>**       m_array;
    array_allocator_t   m_arrayAllocator;
    std::size_t         m_arraySize;
    entry_allocator_t   m_entryAllocator;
    hash_functor_t      m_hasher;
    keyset_allocator_t  m_keyset_allocator;
    HashSet<const K*>   m_keySet;
    std::size_t         m_size;
    valset_allocator_t  m_valset_allocator;
    HashSet<const V*>   m_valueSet;

    Entry<K, V>* allocateEntry(const K& key, const V& value)
    {
        return m_entryAllocator.newObject(Entry<K, V>(key, value));
    }

    inline float calculateLoadFactor() const
    {
        return ((float) m_size) / m_arraySize;
    }

    unsigned hashKey(const K& key) const
    {
        return m_hasher(key) % m_arraySize;
    }

    Entry<K, V>** resizeArray(std::size_t newSize)
    {
        if (m_array == NULL)
        {
            // This is the first allocation
            m_array = m_arrayAllocator.allocate(newSize);
            m_arraySize = newSize;
        }
        else
        {
            // This is a resize allocation

        }
        return m_array;
    }

} ;

}
}

#endif /* HASHMAP_H */

