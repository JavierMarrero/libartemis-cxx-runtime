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
 * File:   Pair.h
 * Author: Javier Marrero
 *
 * Created on December 5, 2022, 7:42 PM
 */

#ifndef PAIR_H
#define PAIR_H

// API
#include <Axf/Core/Object.h>

namespace axf
{
namespace utils
{

/**
 * The pair object encapsulates a key-value pair. It establishes a bidirectional
 * relationship between two objects.
 * <p>
 * The pair object makes it easy to
 *
 * @author J. Marrero
 */
template <typename K, typename V>
class Pair : public core::Object
{
    AXF_CLASS_TYPE(AXF_TEMPLATE_CLASS(axf::utils::Pair<K, V>),
                   AXF_TYPE(axf::core::Object))
public:

    typedef K   keyType;    /// A typedef for the key type of the pair
    typedef V   valueType;  /// A typedef for the value type of the pair

    /**
     * Constructs a new pair object.
     *
     * @param first
     * @param second
     */
    Pair(const K& first, const V& second) : m_first(first), m_second(second) { }

    /**
     * Default destructor
     */
    ~Pair() { };

    /**
     * Retrieves a const reference to the first element of this pair
     *
     * @return
     */
    inline const K& first() const
    {
        return m_first;
    }

    /**
     * Returns a reference to the first element of this pair
     *
     * @return
     */
    inline K& first()
    {
        return m_first;
    }

    /**
     * Retrieves a non const reference to the second element of this pair
     *
     * @return
     */
    inline const V& second() const
    {
        return m_second;
    }

    /**
     * Retrieves a reference to the second element of this pair.
     * 
     * @return
     */
    inline V& second()
    {
        return m_second;
    }

    inline bool operator==(const Pair<K, V>& rhs) const
    {
        return m_first == rhs.m_first && m_second == rhs.m_second;
    }

    inline bool operator!=(const Pair<K, V>& rhs) const
    {
        return m_first != rhs.m_first || m_second != rhs.m_second;
    }

private:

    K   m_first;    /// The first element
    V   m_second;   /// The second element

} ;

template <typename K, typename V>
Pair<K, V> makePair(const K& first, const V& second)
{
    return Pair<K, V>(first, second);
}

}
}

#endif /* PAIR_H */

