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
 * File:   DefaultAllocator.h
 * Author: Javier Marrero
 *
 * Created on December 4, 2022, 12:35 PM
 */

#ifndef DEFAULTALLOCATOR_H
#define DEFAULTALLOCATOR_H

// API
#include <Axf/Collections/Allocator.h>

// C++
#include <climits>

namespace axf
{
namespace collections
{

/**
 * A default allocator object implementation using <code>operator ::new</code>.
 * <p>
 * This allocator is the default memory allocator for the library.
 *
 * @author J. Marrero
 */
template <class T>
class DefaultAllocator : public Allocator<T>
{

    AXF_CLASS_TYPE(axf::collections::DefaultAllocator<T>,
               AXF_TYPE(axf::collections::Allocator<T>))
public:

    T* allocate(typename Allocator<T>::size_type n = 1)
    {
        return reinterpret_cast<T*> (new char[n * sizeof (T)]);
    }

    T* construct(T* p, const T& args)
    {
        return new (p) T(args);
    }

    void destroy(T* p)
    {
        ((T*) (p))->~T();
    }

    void deallocate(T* p, typename Allocator<T>::size_type n)
    {
        delete[] reinterpret_cast<char*> (p);
    }

    virtual typename Allocator<T>::size_type maxSize() const
    {
        return SIZE_MAX;
    }

} ;

}
}

#endif /* DEFAULTALLOCATOR_H */

