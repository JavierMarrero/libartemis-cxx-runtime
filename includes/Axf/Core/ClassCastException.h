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
 * File:   ClassCastException.h
 * Author: Javier Marrero
 *
 * Created on December 3, 2022, 12:20 PM
 */

#ifndef CLASSCASTEXCEPTION_H
#define CLASSCASTEXCEPTION_H

// API
#include <Axf/Core/Exception.h>

namespace axf
{
namespace core
{

/**
 * The <code>ClassCastException</code> exception is thrown whenever a bad
 * casting operation is performed, or when type safety of operations is
 * somehow compromised.
 * <p>
 * 
 * 
 * @author J. Marrero
 */
class ClassCastException : public Exception
{
    AXF_EXCEPTION_TYPE(axf::core::ClassCastException, axf::core::Exception)

public:

    ClassCastException(const char* message);
    ~ClassCastException();

} ;

}
}

#endif /* CLASSCASTEXCEPTION_H */

