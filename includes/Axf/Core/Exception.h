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
 * File:   Exception.h
 * Author: Javier Marrero
 *
 * Created on November 27, 2022, 4:48 PM
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H

// API
#include <Axf/Core/ReferenceCounted.h>

namespace axf
{
namespace core
{
namespace bits
{

/**
 * Exception type descriptors are objects that encapsulate the type information
 * of an exception in order to query it at runtime.
 * <p>
 * It is a separate mechanism than general type introspection provided by the
 * library, since exception types do not derive (at least fundamental exception
 * types) from the <code>axf::core::Object</code> class.
 * <p>
 * However, simple type introspection may be performed on them via this class.
 * More specifically, super-class querying and runtime type name query may be
 * performed.
 * <p>
 * <b>Note:</b> exception types are not destined to be general purpose objects.
 * In order to preserve simplicity of design, we have kept the exception
 * hierarchy scheme as a single inheritance scheme. Therefore, no general
 * runtime super-type querying may be performed on objects, only the primordial
 * super-type may be retrieved.
 * <p>
 * <b>Note:</b> this type may be intrusively reference counted, and it is
 * suitable for its use with the smart pointers.
 *
 * @author J. Marrero
 */
class ExceptionTypeDescriptor : public ReferenceCounted
{
public:

    /**
     * Constructs a new exception type descriptor for a given type with a given
     * super-type.
     * 
     * @param className
     * @param super
     */
    ExceptionTypeDescriptor(const char* className, const ExceptionTypeDescriptor* super);

    /**
     * Default destructor
     */
    ~ExceptionTypeDescriptor();

    /**
     * Returns the name of the class this object describes.
     * 
     * @return
     */
    inline const char* getClassName() const
    {
        return m_className;
    }

    /**
     * Returns true if this exception type describes an exception which is
     * an exact instance of the provided descriptor.
     *
     * @param exceptionType
     * @return
     */
    bool isInstanceOf(const ExceptionTypeDescriptor& exceptionType) const;

    /**
     * Returns true if this exception type describes an exception that is a
     * subtype of the provided parameter.
     * 
     * @param exceptionType
     * @return
     */
    bool isKindOf(const ExceptionTypeDescriptor& exceptionType) const;

    /**
     * Returns the super-type of this class.
     *
     * @return
     */
    const ExceptionTypeDescriptor& super() const;

private:

    const char*                     m_className;    /// The class name
    const ExceptionTypeDescriptor*  m_super;        /// The super type
} ;

}

/**
 * This macro should be used when declaring new exception types. It allows some
 * lighter form of reflection over exception types.
 * <p>
 * Sometimes we will need to query the runtime system about the type of an
 * exception since we will probably not know what type a specific exception is,
 * since we may caught it from a super-type exception.
 */
#define AXF_EXCEPTION_TYPE(Type, Throwable) \
    public: \
        static const axf::core::bits::ExceptionTypeDescriptor& getCompileTimeClass() \
        { \
            static axf::core::bits::ExceptionTypeDescriptor exceptionType(#Type, &Throwable::getCompileTimeClass()); \
            \
            return exceptionType; \
        } \
        \
        inline virtual const char* getClassName() const { return #Type; } \
        \
        inline virtual const axf::core::bits::ExceptionTypeDescriptor& getClass() const \
        { \
            return getCompileTimeClass(); \
        } \
    private:        

/**
 * This is the base class for several types of exceptions. Exceptions are run-time errors that signal abnormal execution
 * conditions.
 * <p>
 * There are several classes of exception, though what they have in common is that they signal <b>errors</b> detected
 * by the program that are normally <b>not recoverable</b>. The use of exceptions as a general control flow mechanism
 * is <b>highly discouraged</b>, since exception dispatching is almost <b>ten</b> times higher more costly than normal
 * return value signaling. Normal <i>C-style</i> return value is preferred for errors that are recoverable and that
 * do not require propagation to higher instances.
 * <p>
 * Exceptions carry a message, which is helpful to developers as well as end users, since they allow to specifically
 * know the cause of the exception. This message is a normal <b>UTF-8</b> encoded string stored in a <code>char</code>
 * pointer.
 * <p>
 * <b>Note</b>: remember that in C++, exception invocation may lead to destructor invocation, possibly deleting objects.
 * 
 * @author J. Marrero
 */
class Exception : public ReferenceCounted
{
public:

    /**
     * Returns the compile-time 'static' type of this exception.
     * 
     * @return
     */
    static const bits::ExceptionTypeDescriptor& getCompileTimeClass();

    Exception(const char* message);     /// Constructor
    virtual ~Exception();               /// Destructor

    /**
     * Returns the polymorphic runtime type descriptor of this object.
     * 
     * @return
     */
    virtual const bits::ExceptionTypeDescriptor& getClass() const;

    /**
     * Returns the polymorphic class name of this type. If the object is
     * accessed through a pointer or reference to the base class, this method
     * will return the class name of the underlying object rather than the
     * pointer´s type.
     * 
     * @return
     */
    virtual const char* getClassName() const;

    /**
     * Returns the message of this exception.
     * 
     * @return 
     */
    inline const char* getMessage() const
    {
        return m_message;
    }

    /**
     * Returns true if this exception object is exactly of the type of the
     * parametric type.
     * 
     * @return
     */
    template <typename E>
    inline bool isInstanceOf()
    {
        return getClass().isInstanceOf(E::getCompileTimeClass());
    }

    /**
     * Returns true if this exception is a type of the parametric type of
     * the function.
     * 
     * @return
     */
    template <typename E>
    inline bool isKindOf()
    {
        return getClass().isKindOf(E::getCompileTimeClass());
    }

private:

    /// The message of this exception
    char m_message[1024];
} ;

}
}

#endif /* EXCEPTION_H */
