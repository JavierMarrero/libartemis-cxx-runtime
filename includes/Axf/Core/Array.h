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
#include <Axf/Core/Object.h>

// C++
#include <cstring>

namespace axf
{
namespace core
{

/**
 * Represents an statically allocated array. Objects of this type may be returned
 * by reference or by value.
 * <p>
 * Arrays are zero-initialized at creation time, which is an O(n) operation. This
 * setting may be overridden by passing the value 'false' as parameter to the
 * constructor. Zero-initialized arrays are obtained with zeros at the memory
 * locations, not necessarily integer or numerical zeros.
 * <p>
 * The class has overloads for the <code>[]</code> operators, and may be passed
 * where pointers are expected.
 * <p>
 * The class inherits <code>ReferenceCounted</code> which makes it suitable for
 * intrusive reference counting. Arrays under this model are objects in all its
 * splendor, and reflexive queries may be used with them.
 * <p>
 * It is possible to ask an array for its underlying type, and use type traits
 * over that type to query information at compile time.
 *
 * @author J. Marrero
 */
template <typename T, unsigned long long SIZE>
class Array : public Object
{
    AXF_CLASS_TYPE(AXF_TEMPLATE_CLASS(axf::core::Array<T, SIZE>), AXF_TYPE(axf::core::Object));

public:

    /**
     * The type of this array.
     */
    typedef T type;

    /**
     * This is the physical length of this array.
     */
    static const size_t length = SIZE;

    /**
     * Length in bytes of the array.
     */
    static const size_t sizeOf = SIZE * sizeof (T);

    /**
     * Default constructor.
     */
    Array(bool zeroInitialize = true)
    {
        if (zeroInitialize)
            std::memset(m_primitive, 0, length * sizeof (T));
    }

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

