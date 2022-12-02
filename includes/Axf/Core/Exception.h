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

namespace axf
{
namespace core
{

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
class Exception
{
public:

    Exception(const char* message);     /// Constructor
    virtual ~Exception();               /// Destructor

    /**
     * Returns the message of this exception.
     * 
     * @return 
     */
    inline const char* getMessage() const
    {
        return m_message;
    }

private:

    /// The message of this exception
    const char* m_message;
} ;

}
}

#endif /* EXCEPTION_H */
