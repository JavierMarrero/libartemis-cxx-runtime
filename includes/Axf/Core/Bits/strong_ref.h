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
 * File:   strong_ref.h
 * Author: Javier Marrero
 *
 * Created on November 29, 2022, 7:01 PM
 */

#ifndef STRONG_REF_H
#define STRONG_REF_H

// API
#include <Axf/Core/Lang-C++/traits.h>
#include <Axf/Core/ReferenceCounted.h>

#include "abstract_ref.h"
#include "memory-dtors.h"

namespace axf
{
namespace core
{

template <typename, typename, typename>
class weak_ref;

/**
 * A <b>strong reference</b> is a smart pointer type that implements reference
 * counting and strong reference owning model.
 * <p>
 * Strong references are the type of references that keep objects alive in a
 * reference counting environment. The other type are weak references, that
 * do not necessarily keep objects alive. Any object with which has an active
 * strong reference is not eligible for garbage collection. The object is garbage
 * collected only when its active references reaches zero.
 * <p>
 * The reference counting model is intrusive for classes descending [ReferenceCounted]
 * (#ReferenceCounted) class; and non intrusive otherwise. Intrusive reference
 * counting is performed directly on the object and therefore it does not have
 * associated allocation nor deallocation costs other than the object's itself.
 * <p>
 * Non intrusive reference counting is done using a separate reference counting
 * block, which holds the actual reference count. The reference counting block
 * is associated to the owning object by the pointer, and it is released upon
 * destruction of the shared object.
 * <p>
 * <b>Note</b>: This operation is not synchronized nor atomic, and therefore,
 * special care must be had if smart pointers are being used in a multi-threaded
 * environment.
 *
 * @author J. Marrero
 */
template <typename T, typename deleter_functor = bits::default_delete<T>,
typename = traits::integral_constant<bool, true > >
class strong_ref : public bits::abstract_ref<T, deleter_functor>
{
    template <typename, typename, typename>
    friend class weak_ref;

public:

    /**
     * Default constructor
     */
    strong_ref() : bits::abstract_ref<T, deleter_functor>(), m_refCount(NULL) { }

    /**
     * Creates a new strong reference and creates the reference counting
     * block as well.
     *
     * @param pointer
     * @param refCount
     */
    strong_ref(T* pointer, refcount_t* refCount = new refcount_t())
    :
    bits::abstract_ref<T, deleter_functor>(pointer), m_refCount(refCount)
    {
        grab();
    }

    /**
     * Default copy constructor.
     *
     * @param rhs
     */
    strong_ref(const strong_ref<T, deleter_functor>& rhs)
    :
    bits::abstract_ref<T, deleter_functor>(rhs.m_pointer), m_refCount(rhs.m_refCount)
    {
        grab();
    }

    /**
     * Releases a reference to the pointed object.
     */
    ~strong_ref()
    {
        release();
    }

    /**
     * Releases a reference to this object while making the pointed address
     * null.
     * <p>
     * This pointed shall no longer point to a valid memory location nor it will
     * track count of any references. The pointer may be reassigned at any point
     * in time.
     */
    inline void reset()
    {
        release();

        // Make null the pointed
        this->m_pointer = NULL;
        this->m_refCount = NULL;
    }

    /**
     * Returns the number of "users" of the pointed object.
     * 
     * @return
     */
    inline size_t users() const
    {
        return (m_refCount != NULL) ? m_refCount->m_strong : 0;
    }

    /**
     * Assignment operator overload.
     *
     * @param rhs
     * @return
     */
    strong_ref<T, deleter_functor>& operator=(const strong_ref<T, deleter_functor>& rhs)
    {
        if (this != &rhs)
        {
            if (rhs.m_pointer != this->m_pointer)
            {
                release();
            }

            this->m_pointer = rhs.m_pointer;
            this->m_refCount = rhs.m_refCount;

            grab();
        }
        return *this;
    }

private:

    refcount_t* m_refCount;     ///< The reference count structure

    /**
     * Grabs a reference to this object.
     */
    inline void grab()
    {
        if (m_refCount != NULL)
        {
            m_refCount->m_strong++;
        }
    }

    /**
     * Releases a reference to this object.
     */
    inline void release()
    {
        if (m_refCount != NULL)
        {
            if (this->m_pointer != NULL && (--m_refCount->m_strong <= 0))
            {
                this->m_disposer(this->m_pointer);
            }
            if (m_refCount->m_strong <= 0 && m_refCount->m_weak <= 0)
            {
                delete m_refCount;
            }
        }
    }
} ;

/**
 * Template specialization for intrusive reference counted classes.
 */
template <typename T>
class strong_ref<T, bits::default_delete<T>, typename is_reference_counted<T>::type>
:
public bits::abstract_ref<T, bits::default_delete<T> >
{
    template <typename, typename, typename>
    friend class weak_ref;

public:

    /**
     * Default constructor.
     *
     * @param pointer
     */
    strong_ref(T* pointer = NULL) : bits::abstract_ref<T, bits::default_delete<T> >(pointer)
    {
        grab();
    }

    /**
     * Default copy constructor.
     * 
     * @param rhs
     */
    strong_ref(const strong_ref<T, bits::default_delete<T> >& rhs)
    :
    bits::abstract_ref<T, bits::default_delete<T> >(rhs.m_pointer)
    {
        grab();
    }

    /**
     * Default destructor.
     */
    virtual ~strong_ref()
    {
        release();
    }

    /**
     * Releases a reference to the currently pointed object and sets the current
     * reference to NULL (invalidates the pointer).
     */
    void reset()
    {
        release();
        this->m_pointer = NULL;
    }

    /**
     * Returns the number of users of this pointer.
     * 
     * @return
     */
    inline size_t users() const
    {
        return this->m_pointer->queryStrongReferences();
    }

private:

    /**
     * Grabs a reference to the pointed object.
     */
    inline void grab()
    {
        if (this->m_pointer != NULL)
        {
            this->m_pointer->grabStrongReference();
        }
    }

    /**
     * Releases a reference to the pointed object. The object deletes itself if
     * the references goes below zero.
     */
    inline void release()
    {
        if (this->m_pointer != NULL)
        {
            this->m_pointer->releaseStrongReference();
        }
        this->m_pointer = NULL;
    }
} ;

}
}

#endif /* STRONG_REF_H */

