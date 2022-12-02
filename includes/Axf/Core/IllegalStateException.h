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
 * File:   IllegalStateException.h
 * Author: Javier Marrero
 *
 * Created on November 28, 2022, 10:41 PM
 */

#ifndef ILLEGALSTATEEXCEPTION_H
#define ILLEGALSTATEEXCEPTION_H

// API
#include <Axf/Core/Exception.h>

namespace axf
{
namespace core
{

/**
 * Signals the runtime environment that the program has reached some sort of
 * illegal state at which computations may not be normally resumed.
 * <p>
 * This exception is raised whenever the computation has reduced to a illegal
 * state. Illegal states include incongruent internal state of classes,
 * data inconsistency, miscarried control flow, bad method invocation, and
 * others.
 * <p>
 * An exception message should be provided in order to clarify the nature of
 * the error.
 *
 * @ref Exception "Exception class"
 * @author J. Marrero
 */
class IllegalStateException : public Exception
{
public:

    IllegalStateException(const char* message);     /// Default constructor
    ~IllegalStateException();                       /// Default destructor

} ;

}
}

#endif /* ILLEGALSTATEEXCEPTION_H */

