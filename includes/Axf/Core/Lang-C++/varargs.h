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
 * File:   varargs.h
 * Author: Javier Marrero
 *
 * Created on December 22, 2022, 8:18 PM
 */

#ifndef SAFE_VARARGS_H
#define SAFE_VARARGS_H

// API
#include <Axf/Core/Memory.h>

// C++
#include <cstddef>
#include <cassert>

namespace axf
{
namespace core
{

template <typename>
class varargs;

namespace __
{

/**
 * This class allocates and deallocates the arrays
 */
class vararg_allocator
{
    template <typename>
    friend class axf::core::varargs;

    static vararg_allocator INSTANCE;   /// The allocator for the variable argument types

    /**
     * This is a singleton class.
     *
     * @return
     */
    inline static const vararg_allocator& get()
    {
        return INSTANCE;
    }

    mutable std::size_t m_capacity;             /// The current pool capacity
    mutable std::size_t m_last;                 /// Last allocation size
    mutable char*       m_pool;                 /// The data pool
    mutable std::size_t m_stackPointer;         /// The stack pointer

    vararg_allocator();
    ~vararg_allocator();

    void*   allocate(std::size_t size) const;
    void    deallocate() const;

    /**
     * Performs a binding allocation. Allocates 'count' elements from the pool.
     * 
     * @param size
     * @return
     */
    template <typename T>
    T* bindAlloc(std::size_t count) const
    {
        return static_cast<T*> (allocate(count * sizeof (T)));
    }
} ;

}

/**
 * Represents a variable argument list. The variable argument list is type safe
 * and its length may be retrieved at runtime. This is designed to substitute
 * the variable argument function calling mechanism in standard C and C++.
 * <p>
 * This variadic function call mechanism is type safe by default. If type
 * safety is undesirable consider using the traditional '...' syntax of the
 * language. However, this provides not only type safety, but also the very
 * desirable bound safety that C variadic arguments lacks.
 * <p>
 * This is nothing more than syntactic sugar. By default, the created list holds
 * space for 128 arguments. If needed, the list may be resized. The variable
 * argument list uses a custom memory allocator, that speeds up allocations,
 * making it equivalent to a stack allocation (at the expense of greater
 * initialization time).
 * <p>
 * As this class uses a custom allocator, its misuse will result in fatal
 * errors that can't be caught, as a lesson to all those morons that doesn't
 * read the documentation that I spend so much time writing.
 *
 * @author J. Marrero
 */
template <typename T>
class varargs
{
    typedef __::vararg_allocator vararg_allocator;

public:

    typedef T type;

    varargs() : m_capacity(0), m_length(0), m_valist(NULL) { }

    varargs(T arg)
    :
    m_capacity(32),
    m_length(1),
    m_valist(vararg_allocator::get().bindAlloc<T>(32))
    {
        m_valist.get()[0] = arg;
    }

    varargs(const varargs<T>& rhs)
    :
    m_capacity(rhs.m_capacity),
    m_length(rhs.m_length),
    m_valist(rhs.m_valist) { }

    /**
     * Default destructor. The reference to the argument list is eliminated
     * once the references pointing to it becomes zero.
     */
    ~varargs() { }

    /**
     * Returns the length of this argument list.
     *
     * @return
     */
    inline std::size_t length() const
    {
        return m_length;
    }

    inline T operator[](std::size_t index) const
    {
        assert(index < m_length && "illegal index (no-throw context)");
        return m_valist.get()[index];
    }

    /**
     * The comma operator allows chaining expressions.
     *
     * @param rhs
     * @return
     */
    inline varargs<T>& operator,(T rhs)
    {
        // Check if resize
        if (m_length == m_capacity)
        {
            grow();
        }
        // Assign
        m_valist.get()[m_length++] = rhs;

        return *this;
    }

private:

    /**
     * Deletes the variable argument list.
     *
     * @param pointer
     */
    struct valist_deletor
    {

        inline void operator()(T* pointer)
        {
            vararg_allocator::get().deallocate();
        }
    } ;

    std::size_t                                 m_capacity; /// The capacity of the variable argument list
    std::size_t                                 m_length;   /// The length of the variable argument list
    axf::core::strong_ref<T, valist_deletor>    m_valist;   /// The list holding the variable arguments

    /**
     * The variable argument list grows in 2's powers. The initial capacity is
     * 32 elements, the list will be resized to hold 64 elements. After that,
     * 128 elements and then 256 elements... The maximum number of elements
     * depends on several factors, one of them being the maximum allocatable
     * space, which is determined by the stack allocator.
     *
     * @param newCount
     */
    void grow()
    {
        m_capacity *= 2;
        T temp[m_capacity / 2];

        for (std::size_t i = 0; i < m_length; ++i)
        {
            temp[i] = m_valist.get()[i];
        }

        // Deallocate and allocate
        vararg_allocator::get().deallocate();
        m_valist = vararg_allocator::get().bindAlloc<T>(m_capacity);

        // Copy back
        for (std::size_t i = 0; i < m_length; ++i)
        {
            m_valist.get()[i] = temp[i];
        }
    }
} ;

/**
 * Creates a new variable argument list, which can be extended after creation
 * thanks to the overload of the ',' operator.
 *
 * @param first
 * @return
 */
template <typename T>
inline varargs<T> make_varargs(T first)
{
    return varargs<T>(first);
}

}
}

#endif /* VARARGS_H */

