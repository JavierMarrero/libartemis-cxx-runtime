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
#include <cstdlib>
#include <cstdio>

// API
#include <Axf/Core/NullPointerException.h>

// Private API
#include "memory-dtors.h"

namespace axf
{
namespace core
{
namespace bits
{

/**
 * This is the abstract base class for all the smart reference types. It provides a default data field of a generic type
 * <code>T*</code> representing the type of the reference. It also provides some common operator overloading and utility
 * methods.
 * <p>
 * 
 * @author J. Marrero
 */
template <typename T, typename deleter_functor = axf::core::bits::default_delete<T> >
class abstract_ref
{
public:

    /**
     * Creates a new abstract reference with a <code>NULL</code> value as pointed object.
     */
    abstract_ref() : m_pointer(NULL) { }

    /**
     * Creates a new abstract reference that points to the provided argument.
     * 
     * @param pointer
     */
    explicit abstract_ref(T* pointer) : m_pointer(pointer) { }

    /// Default destructor

    virtual ~abstract_ref() { }

    /**
     * Returns a reference to the data stored in this object. It is similar to the pointer de-referencing operation.
     *
     * @return a reference to an object of type T
     */
    inline T& asReference()
    {
        checkDereferencingCapability();
        return *m_pointer;
    }

    /**
     * Similar to abstract_ref::getReference but for constant references
     *
     * @return a reference to an object of type T
     */
    inline const T& asReference() const
    {
        checkDereferencingCapability();
        return *m_pointer;
    }

    /**
     * Clears this reference. This method's implementation is overridden by child classes.
     */
    virtual void reset() = 0;

    /**
     * Returns the value of the pointer stored in this reference.
     *
     * @return a pointer of type T
     */
    inline T* get()
    {
        return m_pointer;
    }

    /**
     * Returns the value of the pointer stored in this reference as a const
     * pointer.
     *
     * @return
     */
    inline const T* get() const
    {
        return m_pointer;
    }

    /**
     * Returns true if the pointed object is a null reference.
     * 
     * @return
     */
    inline bool isNull() const
    {
        return m_pointer == NULL;
    }

    /**
     * Returns the truth value of this pointer. It is considered true if the
     * pointed object is not null, false otherwise.
     * 
     * @return
     */
    inline operator bool()
    {
        return m_pointer != NULL;
    }

    /**
     * Two pointers are deemed equals if they point to the same memory location.
     *
     * @param rhs
     * @return
     */
    inline bool operator==(const T* rhs) const
    {
        return m_pointer == rhs;
    }

protected:

    deleter_functor m_disposer;     /// Disposer functor
    T*              m_pointer;      /// A pointer to the data

    /**
     * Essentially checks if a pointer is null or any other invalid value, and
     * if so, throws a <code>NullPointerException</code>. The message is built
     * on the heap.
     *
     * TODO: Check if it would be suitable to statically assign a message buffer.
     * UPDATE: Seems that it is possible
     */
    inline void checkDereferencingCapability()
    {
        char message[64] = {0};
        std::sprintf(message, "null dereferencing from pointer at 0x%16x", this);

        if (m_pointer == NULL)
            throw NullPointerException(message);
    }

} ;

}
}
}

#endif /* ABSTRACT_PTR_H */
