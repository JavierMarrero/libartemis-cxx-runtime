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
 * File:   Allocator.h
 * Author: Javier Marrero
 *
 * Created on December 4, 2022, 11:22 AM
 */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

// API
#include <Axf/Core/Lang-C++/traits.h>
#include <Axf/Core/Object.h>
#include <Axf/Core/OutOfMemoryError.h>

#include <Axf/Core/Lang-C++/traits.h>

// C++
#include <cstddef>
#include <cstring>

namespace axf
{
namespace collections
{
namespace alloc
{

template <std::size_t _TypeSize>
inline std::size_t getSizeOfBlock(std::size_t count)
{
    if (_TypeSize > 1)
    {
        std::size_t maxPossible = static_cast<std::size_t> (-1);

        if (count > maxPossible)
        {
            throw core::OutOfMemoryError("Attempted allocation greater than the maximum", count * _TypeSize);
        }
        return count * _TypeSize;
    }
    return count;
}

inline void* _allocate(std::size_t bytes)
{
    return static_cast<void*> (new char[bytes]);
}

inline void _deallocate(void* ptr)
{
    delete[] (static_cast<char*> (ptr));
}

}

/**
 * This interface defines the named requirement for an allocator object.
 * Allocator objects encapsulates strategies and policies for access/addressing,
 * allocation/deallocation, construction/destruction of objects.
 * <p>
 * Every component of the library (except some very basic components) that have
 * to perform resource management in the form of memory allocations or
 * deallocations does so through this interface.
 * <p>
 * This encapsulation allows to ensure memory management policies impossible
 * to guarantee otherwise. Allocators may be somewhat chained, allowing for
 * different behavior for different memory blocks.
 * <p>
 * This interface allows the implementation of common design patters such as
 * <i>object pool</i>.
 * <p>
 * <b>Note</b>: this interface does not enforces synchronized memory allocation
 * and deallocation. This is responsibility of the programmer, and automatic
 * synchronization may be achieved using wrapper objects (<i>i.e</i> using
 * object composition probably).
 *
 * @author J. Marrero
 */
template <typename T>
class Allocator : public core::Object
{
    AXF_CLASS_TYPE(axf::collections::Allocator<T>,
                   AXF_TYPE(axf::core::Object));
public:

    Allocator() { }

    virtual ~Allocator() { }

    /**
     * Allocates uninitialized storage suitable for storing an array of objects
     * of type T and <i>count</i> elements.
     *
     * @param count
     * @return
     */
    virtual T* allocate(std::size_t count) = 0;

    /**
     * Constructs an element in place. The storage must be already allocated.
     *
     * @param element
     * @return
     */
    virtual T* construct(T* pointer, const T& element) = 0;

    /**
     * Deallocates previously allocated storage. The deallocation does not imply
     * destruction of the elements. The destruction of the elements is
     * performed using the destruct method.
     */
    virtual void deallocate(T* pointer) = 0;

    inline virtual void deleteArray(T* pointer, std::size_t count)
    {
        destruct(pointer, count);
        deallocate(pointer);
    }

    inline virtual void deleteObject(T* pointer)
    {
        destruct(pointer);
        deallocate(pointer);
    }

    /**
     * Destructs an object in place. No storage is deallocated whatsoever. The
     * second parameter is used if an array is passed as pointer. It iterates
     * over the array destroying the objects one on one. No bounds checking is
     * performed.
     *
     * @param pointer
     * @param size
     */
    virtual void destruct(T* pointer, std::size_t count = 1) = 0;

    inline virtual T* newArray(const std::size_t count)
    {
        T* allocatedMemory = allocate(count);
        std::memset(reinterpret_cast<void*> (allocatedMemory), 0, alloc::getSizeOfBlock<sizeof (T)>(count));

        return allocatedMemory;
    }

    inline virtual T* newObject(const T& rhs)
    {
        return construct(allocate(1), rhs);
    }

} ;

}
}

#endif /* ALLOCATOR_H */

