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
 * File:   IllegalArgumentException.h
 * Author: Javier Marrero
 *
 * Created on December 10, 2022, 10:46 AM
 */

#ifndef ILLEGALARGUMENTEXCEPTION_H
#define ILLEGALARGUMENTEXCEPTION_H

// API
#include <Axf/Core/Exception.h>

namespace axf
{
namespace core
{

/**
 * Signals a method has been called with illegal arguments.
 * <p>
 * The constructor of this exception takes a variable number of arguments, and
 * a null terminated character string (<i>i.e.</i> a regular C string) that
 * determines the formatting of the passed arguments. This allows to encapsulate
 * several wrong arguments into one exception message.
 * <p>
 * This constructor is prone to buffer overflow. Any error during construction
 * of this exception object is unspecified behavior.
 * <p>
 * As an example:
 * <pre>
 * int wrongArgumentFoo = -1;
 * 
 * throw IllegalStateException("wrong argument foo: %d",
 * </pre>
 *
 * @author J. Marrero
 */
class IllegalArgumentException : public Exception
{
    AXF_EXCEPTION_TYPE(axf::core::IllegalArgumentException,
                       axf::core::Exception)

public:

    IllegalArgumentException(const char* fmt, ...);
    ~IllegalArgumentException();
} ;

}
}

#endif /* ILLEGALARGUMENTEXCEPTION_H */

