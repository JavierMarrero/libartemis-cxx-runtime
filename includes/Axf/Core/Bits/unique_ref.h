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
 * File:   unique_ptr.h
 * Author: Javier Marrero
 *
 * Created on November 27, 2022, 2:35 AM
 */

#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

// Include the abstract reference
#include "abstract_ref.h"

namespace axf
{
namespace core
{

/**
 * A <b>unique reference</b> is a type of smart pointer that only allows single
 * ownership of a particular reference. The object owned by a
 * <code>unique_ref</code> object has a life cycle never exceeding that of
 * this pointer´s.
 * <p>
 * The semantics of this reference is determined by the quality that this
 * reference owns and manages another object, disposing of it when it goes out
 * of scope.
 * <p>
 * The object is deleted using a deleter functor object when either of the
 * following happens:
 * <ul>
 *  <li>the managing <code>unique_ref</code> is deleted.</li>
 *  <li>the managing <code>unique_ref</code> is assigned to another reference</li>
 * </ul>
 * <p>
 * The pointer is deleted using a <i>(possibly user-supplied)</i> deleter to
 * automatically release memory.
 * <p>
 * This class is not <b>copy assignable</b> nor <b>copy constructible</b>.
 *
 * @author J. Marrero
 */
template <typename T, typename deleter_functor = axf::core::bits::default_delete<T> >
class unique_ref : public bits::abstract_ref<T, deleter_functor>
{
public:

    /**
     * Creates a new instance of a given unique reference pointing to the object
     * passed as parameter.
     * 
     * @param pointer
     */
    unique_ref(T* pointer = NULL) : bits::abstract_ref<T>(pointer) { }

    /**
     * Destroys this object, invoking the destructor of the pointed object.
     */
    ~unique_ref()
    {
        clear();
    }

    /**
     * Deletes the pointed object and clears the pointer. The object is,
     * without exception, deleted, since this pointer semantics implies the
     * ownership of a reference.
     */
    inline void clear()
    {
        if (this->m_pointer != NULL)
        {
            this->m_disposer(this->m_pointer);
            this->m_pointer = NULL;
        }
    }

    /**
     * Assignment operator overload.
     * 
     * @param rhs
     * @return
     */
    inline unique_ref<T, deleter_functor>& operator=(unique_ref<T, deleter_functor>& rhs)
    {
        if (this != &rhs)
        {
            clear();

            // Reassign the pointer
            this->m_pointer = rhs.m_pointer;

            // Clear the second pointer
            rhs.m_pointer = NULL;
        }
        return *this;
    }
    
} ;

}
}

#endif /* UNIQUE_PTR_H */
