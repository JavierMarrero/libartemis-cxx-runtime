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
 * File:   Object.h
 * Author: Javier Marrero
 *
 * Created on November 27, 2022, 1:18 AM
 */

#ifndef OBJECT_H
#define OBJECT_H

// API
#include <Axf/Core/Class.h>
#include <Axf/Core/ReferenceCounted.h>
#include <Axf/Core/String.h>

namespace axf
{
namespace core
{

/**
 * Returns a pointer to the <code>Class</code> object that encapsulates this
 * object's type.
 */
#define AXF_TYPE(...) &__VA_ARGS__::getCompileTimeClass()

/**
 * A macro that encapsulates a multi-parameter template into a single expression.
 * Useful to avoid some nasty errors when passing templates as parameters.
 */
#define AXF_TEMPLATE_CLASS(...) __VA_ARGS__

/**
 * This macro defines an object hierarchy and runtime type information queries
 * meta-data available to all the users of the class.
 * <p>
 * This macro should be first in each class declaration, immediately below the
 * opening curly brace. The first argument is the class itself, and the super
 * types goes next in a comma separated list. One must use the macro
 * <code>AXF_TYPE</code> to pass the super types.
 */
#define AXF_CLASS_TYPE(_Type, ...) \
    public: \
    \
    static const axf::core::Class<_Type >& getCompileTimeClass() \
    { \
        static axf::core::Class<_Type > classVariable(#_Type, __VA_ARGS__, NULL); \
        \
        return classVariable; \
    } \
    \
    private: \
    \
    virtual const axf::core::bits::Type* getRuntimeType() const { return &getCompileTimeClass(); }

/**
 * The object class is the superclass of all objects in the <i>Artemis</i> framework. It provides some common 
 * functionality that enhances interoperability between types. That being said, there will be cases where objects
 * are not derived from this particular class. This detail is implementation dependent, though derivation from
 * <code>Object</code> is strongly encouraged. Normally, objects not deriving from <code>Object</code> are not
 * part of the public API, and rather are private <i>struct/classes</i>.
 * <p>
 * Most important feature of this class hierarchy is the unification of automatic reference counting (via inheritance
 * of the reference counting mechanism) and run-time type information. Therefore, any object deriving this class
 * is susceptible to introspection, type-traits and reference counting via the smart pointers.
 * <p>
 * This class also provides methods <code>toString()</code> and <code>hashCode()</code> methods, which may be used by
 * other classes within this library accordingly.
 * 
 * @author J. Marrero
 */
class Object : public ReferenceCounted
{
public:

    /**
     * Returns a reference to the <code>Class</code> object that describes this
     * object at compile time.
     *
     * @return
     */
    static const axf::core::Class<Object>& getCompileTimeClass();

    Object();                       /// Constructs a new instance of an <code>Object</code>
    virtual ~Object();              /// Destructs this object

    /**
     * Performs the equality check between two objects. 
     * <p>
     * This method may be overridden by child classes in order to implement
     * custom comparison behavior. If this is the actual case, then, the
     * overriding implementation must guarantee these axioms:
     * <ul>
     *  <li>It must be reflexive: <code>x == x</code></li>
     *  <li>It must be symmetric: <code>x == y</code> must be equals to <code>y == x</code></li>
     *  <li>It must be transitive: if <code>x == y</code> and <code>y == z</code>
     *      then <code>x == z</code></li>
     * </ul>
     * 
     * @return a boolean value indicating whether the two comparing objects are equals.
     */
    virtual bool equals(const Object& object) const;

    /**
     * Returns the <code>Class</code> object associated with this type at
     * runtime.
     * <p>
     * This method is polymorphic and will always return the type of the
     * underlying object, rather than the calling pointer.
     * 
     * @return
     */
    template <typename T>
    inline const Class<T>& getClass() const
    {
        return reflection::asClassUnsafe<T>(*(getRuntimeType()));
    }

    /**
     * Returns a unique 32-bit integer identifying this object.
     * 
     * @return
     */
    virtual int hashCode() const;

    /**
     * Returns a (possibly human readable) representation of this object.
     * <p>
     * The default implementation returns the class name, appended to the
     * address of the calling object.
     * 
     * @return a new String object representing this object
     */
    virtual string toString() const;

private:

    /**
     * Returns the runtime type object associated with this.
     * 
     * @return
     */
    virtual const bits::Type* getRuntimeType() const;

} ;

}
}

#endif /* OBJECT_H */
