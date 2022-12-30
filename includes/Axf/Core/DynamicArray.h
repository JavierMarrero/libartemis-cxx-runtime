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
 * File:   DynamicArray.h
 * Author: Javier Marrero
 *
 * Created on December 27, 2022, 10:52 PM
 */

#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

// API
#include <Axf/Core/Object.h>
#include <Axf/Core/IndexOutOfBoundsException.h>

// C++
#include <cstddef>

namespace axf
{
namespace core
{

/**
 * A dynamic array represents an array that is stored on the heap instead of the
 * stack. This class performs resource allocation and deallocation dynamically,
 * so one does not have to concern about this.
 * <p>
 * 
 *
 * @return
 */
template <typename T>
class DynamicArray : public Object
{

    AXF_CLASS_TYPE(axf::core::DynamicArray<T>, AXF_TYPE(axf::core::Object))
public:

    /**
     * Creates a new dynamic array.
     *
     * @param length
     */
    DynamicArray(std::size_t length)
    : m_array(new T[length]), m_length(length) { }

    /**
     * Destroys the array, releasing the memory allocated.
     */
    ~DynamicArray()
    {
        delete[] m_array;
    }

    /**
     * Decays this array into a pointer for pointer related operations. This is
     * normally necessary when calling old C and C++ functions that acquire
     * primitive arrays arguments.
     * 
     * @return
     */
    inline T* decay()
    {
        return m_array;
    }

    /**
     * Returns length.
     *
     * @return
     */
    inline std::size_t length() const
    {
        return m_length;
    }

    /**
     * Returns this array decayed as pointer, for implicit conversion facilities.
     *
     * @return
     */
    inline operator T*()
    {
        return m_array;
    }

    /**
     * Returns this array decayed as const pointer, for implicit conversion facilities.
     *
     * @return
     */
    inline operator const T*() const
    {
        return m_array;
    }

    /**
     * Equivalent to the [] operation on an array.
     *
     * @param index
     * @return
     */
    inline T& operator[](std::size_t index)
    {
        return m_array[index];
    }

    /**
     * Equivalent to the [] operation on a const reference to an array.
     *
     * @param index
     * @return
     */
    inline const T& operator[](std::size_t index) const
    {
        return m_array[index];
    }

private:

    T*          m_array;    /// The array itself
    std::size_t m_length;   /// The length

    void checkIndexIsValid(std::size_t index) const
    {
        if (index >= m_length)
            throw core::IndexOutOfBoundsException("invalid access to index.", index);
    }
} ;

}
}

#endif /* DYNAMICARRAY_H */

