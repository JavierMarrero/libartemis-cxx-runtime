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
 * File:   weak_ref.h
 * Author: Javier Marrero
 *
 * Created on November 30, 2022, 7:45 AM
 */

#ifndef WEAK_REF_H
#define WEAK_REF_H

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
class strong_ref;

/**
 * A weak reference object is a type of soft reference that is always eligible
 * for garbage collection if there are not strong references pointing to the
 * object.
 * <p>
 * Weak references are used to model <p>"child-to-parent"</p> relationships
 * or where the pointed object may die without further notice. This is used
 * advantageously to break reference cycles. A reference cycle occurs when
 * a mutual referencing is made by two objects, A and B, and neither allows
 * the release of the other once their supposed life cycle is terminated. These
 * kind of situations are bugs that commonly induce memory leaks.
 * <p>
 * There are two classes of <code>weak_ref</code> objects: intrusive and
 * non-intrusive.
 *
 * @author J. Marrero
 */
template <typename T, typename deleter_functor = bits::default_delete<T>,
typename = traits::integral_constant<bool, true > >
class weak_ref : public bits::abstract_ref<T, deleter_functor>
{
    template <typename, typename, typename>
    friend class strong_ref;

public:

    /**
     * Default constructor
     */
    weak_ref() : bits::abstract_ref<T, deleter_functor>(NULL), m_refCount(NULL) { }

    /**
     * Default parametric constructor.
     * 
     * @param pointer
     * @param refCount
     */
    weak_ref(T* pointer, refcount_t* refCount = new refcount_t())
    :
    bits::abstract_ref<T, deleter_functor>(pointer),
    m_refCount(refCount)
    {
        grab();
    }

    /**
     * Default converting constructor.
     * 
     * @param rhs
     */
    weak_ref(const strong_ref<T, deleter_functor>& rhs)
    :
    bits::abstract_ref<T, deleter_functor>(rhs.m_pointer),
    m_refCount(rhs.m_refCount)
    {
        grab();
    }

    /**
     * Default copy constructor.
     * 
     * @param rhs
     */
    weak_ref(const weak_ref<T, deleter_functor>& rhs)
    :
    bits::abstract_ref<T, deleter_functor>(rhs.m_pointer),
    m_refCount(rhs.m_refCount)
    {
        grab();
    }

    /**
     * Default destructor.
     */
    virtual ~weak_ref()
    {
        release();
    }

    /**
     * Resets this reference.
     * <p>
     * Makes it null, and releases a weak reference.
     */
    inline void reset()
    {
        release();

        // Make everything null
        this->m_pointer = NULL;
        this->m_refCount = NULL;
    }

    /**
     * Returns the count of weak users of this object.
     *
     * @return
     */
    inline size_t users() const
    {
        return m_refCount != NULL ? m_refCount->m_weak : 0;
    }

    /**
     * Assignment operator overload.
     *
     * @param rhs
     * @return
     */
    inline weak_ref<T, deleter_functor>& operator=(const weak_ref<T, deleter_functor>& rhs)
    {
        if (this != &rhs)
        {
            release();

            this->m_pointer = rhs.m_pointer;
            this->m_refCount = rhs.m_refCount;

            grab();
        }
        return *this;
    }

private:

    refcount_t* m_refCount;     /// the reference counting variable

    /**
     * Grabs a reference
     */
    inline void grab()
    {
        if (m_refCount != NULL)
            ++(m_refCount->m_weak);
    }

    /**
     * Releases a reference
     */
    inline void release()
    {
        if (m_refCount != NULL)
        {
            if (this->m_pointer != NULL && (--m_refCount->m_weak <= 0) && (m_refCount->m_strong == 0))
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
 * Default template specialization for intrusive types.
 */
template <typename T>
class weak_ref<T, bits::default_delete<T>, typename is_reference_counted<T>::type>
:
public bits::abstract_ref<T>
{
    template <typename, typename, typename>
    friend class strong_ref;

public:

    /**
     * Default constructor
     */
    weak_ref() : bits::abstract_ref<T>(NULL) { }

    /**
     * Default parametric constructor.
     *
     * @param pointer
     * @param refCount
     */
    weak_ref(T* pointer, refcount_t* refCount = new refcount_t())
    :
    bits::abstract_ref<T>(pointer)
    {
        grab();
    }

    /**
     * Default copy constructor.
     *
     * @param rhs
     */
    weak_ref(const weak_ref<T>& rhs)
    :
    bits::abstract_ref<T>(rhs.m_pointer)
    {
        grab();
    }

    /**
     * Default converting constructor for intrusive strong references.
     * 
     * @param rhs
     */
    weak_ref(const strong_ref<T>& rhs)
    :
    bits::abstract_ref<T>(rhs.m_pointer)
    {
        grab();
    }

    /**
     * Default destructor
     */
    virtual ~weak_ref()
    {
        release();
    }

    /**
     * Resets this reference.
     * <p>
     * Makes it null, and releases a weak reference.
     */
    inline void reset()
    {
        release();

        // Make everything null
        this->m_pointer = NULL;
    }

    /**
     * Returns the count of weak users of this object.
     *
     * @return
     */
    inline size_t users() const
    {
        return this->m_pointer != NULL ? this->m_pointer->queryWeakReferences() : 0;
    }

    /**
     * Assignment operator overload.
     *
     * @param rhs
     * @return
     */
    inline weak_ref<T>& operator=(const weak_ref<T>& rhs)
    {
        if (this != &rhs)
        {
            release();

            this->m_pointer = rhs.m_pointer;

            grab();
        }
        return *this;
    }

private:

    inline void grab()
    {
        if (this->m_pointer != NULL)
            this->m_pointer->grabWeakReference();
    }

    inline void release()
    {
        if (this->m_pointer != NULL)
            this->m_pointer->releaseWeakReference();
    }
} ;

}
}

#endif /* WEAK_REF_H */

