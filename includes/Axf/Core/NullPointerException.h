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
 * File:   NullPointerException.h
 * Author: Javier Marrero
 *
 * Created on November 27, 2022, 5:05 PM
 */

#ifndef NULLPOINTEREXCEPTION_H
#define NULLPOINTEREXCEPTION_H

// API
#include <Axf/Core/Exception.h>

namespace axf
{
namespace core
{

/**
 * A <b>null pointer dereferencing</b> is a highly risky type of error that
 * arises whenever the programmer dereferences a pointer that points to an
 * invalid memory location.
 * <p>
 * This is normally a fatal error that results in a crash of the application.
 * However, smart pointers will throw this exception signaling a <i>null-pointer
 * dereferencing</i>, and handlers will be allowed to recover from this
 * abnormal condition.
 *
 * @author J. Marrero
 */
class NullPointerException : public Exception
{
public:

    NullPointerException(const char* message);
    ~NullPointerException();                    /// The destructor is marked non-virtual on purpose

} ;

}
}

#endif /* NULLPOINTEREXCEPTION_H */

