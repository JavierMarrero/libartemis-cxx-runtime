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
 * File:   Array.h
 * Author: Javier Marrero
 *
 * Created on November 28, 2022, 8:11 PM
 */

#ifndef ARRAY_H
#define ARRAY_H

// API
#include <Axf/Core/IndexOutOfBoundsException.h>

namespace axf
{
namespace core
{

template <typename T, unsigned long long SIZE>
class Array
{
public:

    /**
     * This is the physical length of this array.
     */
    static const size_t length = SIZE;

    /**
     * Default constructor.
     */
    Array() { }

    /**
     * Copy the contents of the array 'rhs' to this array. This array´s length
     * must be equals than the passed array.
     * 
     * @param rhs
     */
    Array(const Array<T, SIZE>& rhs)
    {
        for (size_t i = 0; i < length; ++i)
        {
            m_primitive[i] = rhs.m_primitive[i];
        }
    }

    /**
     * Default destructor
     */
    ~Array() { }

    /**
     * The <code>at</code> method returns a reference to the i<sup>th</sup>
     * element in this array. The method is bound-checked, raising an exception
     * when an error is detected.
     *
     * @param index
     * @return
     */
    inline const T& at(size_t index) const
    {
        checkIndexExclusive(index);
        return m_primitive[index];
    }

    /**
     * Non const version of the <code>at</code> method of this class.
     * 
     * @param index
     * @return
     */
    inline T& at(size_t index)
    {
        checkIndexExclusive(index);
        return m_primitive[index];
    }

    /**
     * Returns a non-mutative view of the i<sup>th</sup> element of this array.
     * 
     * @param index
     * @return
     */
    inline const T& operator[](size_t index) const
    {
        return at(index);
    }

    /**
     * Returns a mutative view of the i<sup>th</sup> element of this array.
     * 
     * @param index
     * @return
     */
    inline T& operator[](size_t index)
    {
        return at(index);
    }

private:

    T m_primitive[SIZE];

    /**
     * Checks for the index to be lesser than the array size.
     * 
     * @param index
     */
    void checkIndexExclusive(size_t index)
    {
        if (index >= SIZE)
        {
            throw IndexOutOfBoundsException("attempted to access array with invalid index.", index);
        }
    }

} ;

}
}

#endif /* ARRAY_H */

