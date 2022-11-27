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
 * File:   abstract_ptr.h
 * Author: Javier Marrero
 *
 * Created on November 27, 2022, 2:37 AM
 */

#ifndef ABSTRACT_PTR_H
#define ABSTRACT_PTR_H

// C++
#include <cstddef>

namespace axf
{
namespace core
{
namespace bits
{

/**
 * This is the abstract base class for all the smart reference types.
 */
template <typename T>
class abstract_ref
{
public:

    abstract_ref()
    {
        this->m_pointer = NULL;
    }

    abstract_ref(T* pointer)
    {
        this->m_pointer = pointer;
    }

    virtual ~abstract_ref()
    {
        this->m_pointer = NULL;
    }
    
    /**
     * Clears this reference.
     */
    virtual void clear() = 0;

protected:

    T* m_pointer;
};

}
}
}

#endif /* ABSTRACT_PTR_H */
