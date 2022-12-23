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
 * File:   VarargsSafe.cpp
 * Author: Javier Marrero
 * 
 * Created on December 23, 2022, 7:36 AM
 */

#include <Axf/Core/Lang-C++/varargs.h>
#include <Axf/Core/IllegalStateException.h>
#include <Axf/Core/NullPointerException.h>

// DEBUG
#define DEBUG 0
// -----

// C
#include <cstdlib>

#if DEBUG == 1
#    include <stdio.h>
#endif

#define VARARG_STACK_DEFAULT_SIZE   0x4000      // 16KiB stack
#define VARARG_STACK_SIZE_LIMIT     0x200000    // 2MiB stack

using namespace axf::core::__;

vararg_allocator vararg_allocator::INSTANCE;

vararg_allocator::vararg_allocator()
:
m_capacity(VARARG_STACK_DEFAULT_SIZE),
m_last(0),
m_pool(static_cast<char*> (std::calloc(VARARG_STACK_DEFAULT_SIZE, sizeof (char)))),
m_stackPointer(0)
{
    if (m_pool == NULL)
        throw NullPointerException("unable to allocate variadic arguments pool.");
}

vararg_allocator::~vararg_allocator()
{
    if (m_pool != NULL)
        std::free(m_pool);
}

void* vararg_allocator::allocate(std::size_t size) const
{
    // Check if an allocation will surpass limits
    if (m_stackPointer + size + sizeof (std::size_t) > VARARG_STACK_SIZE_LIMIT)
    {
        throw IllegalStateException("surpassed allowed limit for variadic stack resizing.");
    }

    // Resize if necessary
    if (m_stackPointer + size + sizeof (std::size_t) > m_capacity)
    {
        // This grows in 2's powers
        m_capacity *= 2;

        char* newPool = static_cast<char*> (std::malloc(m_capacity));
        if (newPool == NULL)
        {
            throw NullPointerException("unable to allocate new variadic argument pool!");
        }

        for (std::size_t k = 0; k < m_stackPointer; ++k)
        {
            newPool[k] = m_pool[k];
        }

        std::free(m_pool);
        m_pool = newPool;
    }

    // Allocate in O(1) time
    m_last = size;
    m_stackPointer += size + sizeof (std::size_t);

    // Place the marker
    std::size_t* marker = reinterpret_cast<std::size_t*> (m_pool + (m_stackPointer - sizeof (std::size_t)));
    *marker = m_last;

#if DEBUG
    printf("%s -> allocating %llu bytes (marker: %llu)\n", __func__, size, *marker);
#endif

    return static_cast<void*> (m_pool + (m_stackPointer - m_last));
}

void vararg_allocator::deallocate() const
{
    if (m_stackPointer - (m_last + sizeof (std::size_t)) < 0)
    {
        throw IllegalStateException("somehow stack pointer got below permitted value.");
    }

#if DEBUG
    printf("%s -> deallocating %llu bytes\n", __func__, m_last);
#endif

    m_stackPointer -= m_last + sizeof (std::size_t);

    // Get the last marker
    if (m_stackPointer > 0)
        m_last = *((std::size_t*)((m_pool + m_stackPointer) - sizeof (std::size_t)));

#if DEBUG
    printf("%s -> new marker is %llu bytes.\n", __func__, m_last);
#endif
}

