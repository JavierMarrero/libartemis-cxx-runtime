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
 * File:   reference-wrapper.h
 * Author: Javier Marrero
 *
 * Created on December 11, 2022, 11:16 PM
 */

#ifndef REFERENCE_WRAPPER_H
#define REFERENCE_WRAPPER_H

// API
#include <Axf/Core/Lang-C++/keywords.h>
#include <Axf/Core/NullPointerException.h>

// C++
#include <cstddef>

namespace axf
{
namespace core
{

/**
 * Wraps a reference, in a copyable and assignable object, allowing to use
 * references in classes where they would not be allowed.
 * <p>
 * Specifically, reference wrapper is a <i>copy-constructible</i> and
 * <i>copy-assignable</i> wrapper around a reference of type <i>T</i>. Instances
 * of <code>reference_wrapper</code> are objects, so that they can be stored
 * in containers, but they are also implicitly convertible to <i>T&</i> so that
 * they can be used as arguments to functions that take the type <i>T&</i> as
 * argument.
 * <p>
 * See also the helper functions that helps creating <code>reference_wrapper</code>
 * objects.
 */
template <typename T>
class reference_wrapper
{
public:

    typedef T type; /// The type of this wrapper

    reference_wrapper() : m_wrapped(NULL) { }

    /**
     * Constructs a new reference wrapper object from a reference.
     *
     * @param wrapped
     */
    reference_wrapper(const T& wrapped) : m_wrapped((T*) &wrapped) { }

    /**
     * Creates a new reference wrapper from a previous reference wrapper.
     *
     * @param rhs
     */
    reference_wrapper(const reference_wrapper<T>& rhs) : m_wrapped(rhs.m_wrapped) { }

    /**
     * Assignment operator overload.
     *
     * @param rhs
     * @return
     */
    reference_wrapper& operator=(const reference_wrapper<T>& rhs)
    {
        m_wrapped = rhs.m_wrapped;
        return *this;
    }

    // Access

    inline operator T&() const
    {
        checkDereferencingCapability();
        return *m_wrapped;
    }

    /**
     * Returns the wrapped reference.
     * 
     * @return
     */
    inline T& get() const
    {
        checkDereferencingCapability();
        return *m_wrapped;
    };

    /**
     * Checks if two wrappers point to the same reference.
     *
     * @param rhs
     * @return
     */
    inline bool operator==(const reference_wrapper<T>& rhs) const
    {
        return *m_wrapped == *rhs.m_wrapped;
    }

    /**
     * Checks if two wrapper points to different references.
     *
     * @param rhs
     * @return
     */
    inline bool operator!=(const reference_wrapper<T>& rhs) const
    {
        return *m_wrapped != *rhs.m_wrapped;
    }

private:

    T*  m_wrapped;

    inline void checkDereferencingCapability() const
    {
        if (m_wrapped == NULL)
            throw NullPointerException("attempted to dereference a null pointer from a wrapped reference.");
    }
} ;

/**
 * Returns a reference to an object.
 *
 * @param wrapped
 * @return
 */
template <typename T>
reference_wrapper<T> ref(T& wrapped) noexcept
{
    return reference_wrapper<T>(wrapped);
}

/**
 * Returns a constant reference to an object.
 *
 * @param wrapped
 * @return
 */
template <typename T>
reference_wrapper<const T> const_ref(const T& wrapped) noexcept
{
    return reference_wrapper<const T>(wrapped);
}

}
}

#endif /* REFERENCE_WRAPPER_H */

