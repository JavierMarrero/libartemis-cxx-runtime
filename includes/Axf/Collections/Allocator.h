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

// C++
#include <cstddef>

namespace axf
{
namespace collections
{

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

    typedef T           value_type;     /// The type of objects allocated by this allocator
    typedef T*          pointer;        /// A pointer to the object type of this allocator
    typedef const T*    const_pointer;  /// A const pointer to the object type of this allocator
    typedef T&          reference;      /// A reference to the object type of this allocator
    typedef const T&    const_reference; /// A const reference to the object type of this allocator
    typedef std::size_t size_type;      /// Size type


    /**
     * Allocates storage suitable for an array object of type T[n] and creates
     * the array but does not construct array elements. This means the storage
     * is uninitialized.
     * <p>
     * The method may throw exceptions at will. If n is equals to zero, the
     * result value is unspecified (commonly a NULL pointer).
     *
     * @param n
     * @return
     */
    virtual Allocator<T>::pointer  allocate(Allocator<T>::size_type n = 1) = 0;

    /**
     * Constructs an object of type <code>T</code> in previously allocated
     * storage at address pointed to by <code>xp</code> using <code>args</code>
     * as the constructor arguments.
     *
     * @param p
     * @param args
     */
    virtual Allocator<T>::pointer construct(Allocator<T>::pointer p, Allocator<T>::const_reference args) = 0;

    /**
     * Releases storage pointed to <code>p</code>. <code>p</code> must be
     * a pointer obtained by a previous call to <code>allocate</code> that
     * has not been invalidated by a previous call to <code>deallocate</code>.
     * <p>
     * The method is guaranteed to not to throw exceptions. If a null pointer
     * is passed, the deallocation request is ignored.
     * <p>
     * <code>n</code> must be an integer matching the previous value passed
     * to <code>allocate</code>. Some implementations may choose to ignore
     * this parameter.
     *
     * @param p
     */
    virtual void deallocate(Allocator<T>::pointer p, Allocator<T>::size_type n = 1) = 0;

    /**
     * Destroys the object pointed by <code>p</code> and releases the storage
     * assigned to this object.
     * <p>
     * This method does not throw any exception.
     *
     * @param ptr
     */
    void deleteObject(Allocator<T>::pointer p)
    {
        if (p != NULL)
        {
            destroy(p);
            deallocate(p);
        }
    }

    /**
     * Destroys an object of type <code>T</code> pointed to by <code>xp</code>
     * but does not deallocate any storage.
     * 
     * @param p
     */
    virtual void destroy(Allocator<T>::pointer p) = 0;

    /**
     * Returns the max size that can allocate this allocator within a single
     * allocation request.
     *
     * @return
     */
    virtual Allocator<T>::size_type maxSize() const = 0;

    /**
     * Constructs a new object object. The object must be copy constructible,
     * for this method to work properly.
     * <p>
     * This method throws an <code>OutOfMemoryError</code> exception if the
     * system (or the allocator if it is not system-wide) runs out of memory.
     *
     * @param args
     * @return
     */
    T* newObject(Allocator<T>::const_reference args)
    {
        T* memory = allocate();
        if (memory == NULL)
        {
            throw axf::core::OutOfMemoryError("unable to satisfy allocation request because of memory exhaustion.");
        }
        return construct(memory, args);
    }

} ;

}
}

#endif /* ALLOCATOR_H */

