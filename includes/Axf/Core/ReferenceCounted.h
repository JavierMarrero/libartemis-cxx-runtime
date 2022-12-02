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
 * File:   ReferenceCounted.h
 * Author: Javier Marrero
 *
 * Created on November 28, 2022, 5:19 PM
 */

#ifndef REFERENCECOUNTED_H
#define REFERENCECOUNTED_H

// API
#include <Axf/Core/Lang-C++/traits.h>
#include <Axf/Core/IllegalStateException.h>

namespace axf
{
namespace core
{

/**
 * This structure holds the actual reference count of an object. There are
 * currently two types of references:
 * <ul>
 *  <li>strong references</li>
 *  <li>weak references</li>
 * </ul>
 * <p>
 * Strong references are those that have authority to keep an object alive in a
 * reference counted system. Smart pointer implementations will guarantee that
 * no object with positive reference count will be deleted, and those with zero
 * or less strong references (less would imply an error, but more on that later)
 * will be destroyed (regardless of their weak reference count).
 * <p>
 * Weak references, on the other hand, are quite less powerful in the sense that
 * they don't have the authority to keep objects alive (nor to kill them, for
 * what it's worth). If an object reaches zero or negative weak references,
 * a check for strong references is made. If the strong references are null,
 * then the object is destroyed.
 * <p>
 * One could ask why are references implemented as signed integers. Though
 * negative reference counting should <b>not</b> happen, it can actually
 * happen (in multi-threaded applications for example, since reference increase
 * or decrease is not protected by any synchronization mechanism, in order to
 * squeeze performance). The most reliable way to check for these kind of errors
 * is manual probing (done by the smart pointers). Therefore, references are
 * implemented as 32-bit long signed integer numbers, yielding a total of
 * 2147483647 references. This should be enough for most use cases.
 * <p>
 * In some architectures, long values are 64-bit long, therefore, the number of
 * available references is increased exponentially. 
 */
typedef struct refcount
{
    volatile long m_strong;  /// The count of strong references
    volatile long m_weak;    /// The count of weak references
} refcount_t;

/**
 * Initializes a reference count block structure to zero.
 * 
 * @param rc
 * @return rc
 */
refcount_t& init_refcount(refcount_t& rc);

/**
 * Intrusive reference counting for objects and users of the public API.
 * <p>
 * This class allows fast intrusive reference counting for objects that derive
 * of it. The main reason for intrusive reference counting to be faster than
 * non-intrusive reference counting is that there are no dynamic memory allocations
 * for reference counting blocks.
 * <p>
 * Normally, a reference counted object will have two types of references: weak
 * and strong. However, holding the actual values is responsibility of the
 * reference counting block structure. However, this class exposes a interface
 * that enforces the existence of at least strong and weak references.
 *
 * @author J. Marrero
 */
class ReferenceCounted
{
public:

    ReferenceCounted() : m_references(init_refcount(m_references)) { }
    virtual ~ReferenceCounted();

    /**
     * Increases the strong reference counting of this object by one.
     */
    inline void grabStrongReference() const
    {
        m_references.m_strong++;
    }

    /**
     * Increases the weak reference counting of this object by one.
     */
    inline void grabWeakReference() const
    {
        m_references.m_weak++;
    }

    /**
     * Returns a constant reference to the reference counting structure.
     * 
     * @return a refcount_t const reference
     */
    inline const refcount_t& queryRefcount() const
    {
        return m_references;
    }

    /**
     * Returns the count of strong references pointing to this object.
     * 
     * @return a long integer representing the value of the strong reference.
     */
    inline long queryStrongReferences() const
    {
        return m_references.m_strong;
    }

    /**
     * Returns the count of weak references pointing to this object.
     * 
     * @return a long integer representing the value of the weak reference.
     */
    inline long queryWeakReferences() const
    {
        return m_references.m_weak;
    }

    /**
     * Releases a strong reference of this object. If the strong reference
     * count reaches zero, the object deletes itself.
     */
    inline void releaseStrongReference() const
    {
        if (m_references.m_strong == 0)
            throw IllegalStateException("attempted to release a reference of an already deleted object.");

        if (--m_references.m_strong <= 0)
        {
            delete this;
        }
    }

    /**
     * Releases a weak reference from this object.
     */
    inline void releaseWeakReference() const
    {
        if (m_references.m_weak == 0)
            throw IllegalStateException("attempted to release a reference of an already deleted object.");

        if (--m_references.m_weak <= 0 && (m_references.m_strong == 0))
        {
            delete this;
        }
    }

protected:

    /**
     * Queries and returns a reference to the "reference counting block" structure
     * that actually holds this object's reference count.
     * 
     * @return a reference to a struct of type "refcount_t"
     */
    inline refcount_t& queryRefcount()
    {
        return m_references;
    }

private:

    mutable refcount_t m_references;    /// This field is mutable since it may be used with const objects
} ;

/**
 * This type trait allows to determinate whether a determined data type is an instance
 * of ReferenceCounted. This is used in several contexts to determine whether
 * intrusive reference counting must be used.
 * <p>
 * This type trait's evaluation result is computed in compile time and therefore
 * there are no runtime penalties implied in the evaluation of this expression,
 * other than slower compile times.
 *
 * @author J. Marrero
 */
template <typename T>
struct is_reference_counted : public traits::integral_constant<bool, traits::is_base_of<ReferenceCounted, T>::value>
{
} ;

}
}

#endif /* REFERENCECOUNTED_H */

