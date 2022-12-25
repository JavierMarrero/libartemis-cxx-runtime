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
 * File:   Map.h
 * Author: Javier Marrero
 *
 * Created on December 6, 2022, 6:20 PM
 */

#ifndef MAP_H
#define MAP_H

// API
#include <Axf/Collections/Allocator.h>
#include <Axf/Collections/Collection.h>
#include <Axf/Collections/DefaultAllocator.h>
#include <Axf/Collections/Iterator.h>
#include <Axf/Collections/Set.h>
#include <Axf/Core/Lang-C++/reference-wrapper.h>
#include <Axf/Core/Memory.h>

namespace axf
{
namespace collections
{

/**
 * This interface represents an entry in a map object. Entries are the
 * fundamental building blocks of maps, since they provide the basic association.
 *
 * @author J. Marrero
 */
template <typename K, typename V>
class Entry : public axf::core::Object
{

    AXF_CLASS_TYPE(AXF_TEMPLATE_CLASS(axf::collections::Entry<K, V>),
                   AXF_TYPE(axf::core::Object))

public:

    Entry(const K& key, const V& value) : m_key(key), m_value(value) { }

    ~Entry() { }

    inline const K& getKey() const
    {
        return m_key;
    }

    inline V& getValue()
    {
        return m_value;
    }

    inline const V& getValue() const
    {
        return m_value;
    }

    inline virtual bool equals(const Object& object) const
    {
        return keyMatches(static_cast<const Entry<K, V>&> (object));
    }

    /**
     * Returns true if two keys matches exactly.
     *
     * @param rhs
     * @return
     */
    inline bool keyMatches(const Entry<K, V>& rhs) const
    {
        return m_key == rhs.m_key;
    }

protected:

    K   m_key;      // The key of the entry
    V   m_value;    // The value of the entry

} ;

/**
 * The <code>Map</code> interface sets up a key-value relationship collection.
 * Unique keys are associated to unique values
 */
template <typename K, typename V>
class Map : public Collection<Entry<K, V> >
{
    AXF_CLASS_TYPE(AXF_TEMPLATE_CLASS(axf::collections::Map<K, V>),
                   AXF_TYPE(AXF_TEMPLATE_CLASS(axf::collections::Collection<Entry<K, V> >)));

public:

    /**
     * Default destructor.
     */
    virtual ~Map() { }

    /**
     * Returns a boolean indicating whether the provided key is contained
     * inside the map.
     *
     * @param key
     * @return
     */
    virtual inline bool containsKey(const K& key) const
    {
        return keySet().contains(key);
    }

    /**
     * Returns a boolean indicating whether the provided value is contained
     * within the map.
     *
     * @param value
     * @return
     */
    virtual inline bool containsValue(const V& value) const
    {
        return values().contains(value);
    }

    virtual V& get(const K& key) const = 0;

    /**
     * Returns a set of the keys contained within this map.
     *
     * @return
     */
    virtual const Set<core::reference_wrapper<const K> >& keySet() const = 0;

    /**
     * Puts an key-value pair into the hash map. If the key already exists,
     * replaces the value at the key value pair.
     *
     * @param key
     * @param value
     * @return
     */
    virtual bool put(const K& key, const V& value) = 0;

    /**
     * Removes the pair of elements that contains the key "key".
     *
     * @param key
     * @return
     */
    virtual bool remove(const K& key) = 0;

    /**
     * Returns a collection that iterates over all the values of the map.
     *
     * @return
     */
    virtual const Collection<core::reference_wrapper<V> >& values() const = 0;

} ;

}
}

#endif /* MAP_H */

