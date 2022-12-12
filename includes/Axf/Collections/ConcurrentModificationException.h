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
 * File:   ConcurrentModificationException.h
 * Author: Javier Marrero
 *
 * Created on December 10, 2022, 10:32 AM
 */

#ifndef CONCURRENTMODIFICATIONEXCEPTION_H
#define CONCURRENTMODIFICATIONEXCEPTION_H

// API
#include <Axf/Core/Exception.h>

namespace axf
{
namespace collections
{

/**
 * Exception that is thrown when a collection detected that a modification has
 * been made when it is not allowed, such as when an iterator is operating over
 * the collection and the collection is structurally modified.
 * <p>
 * An iterator that detects this condition is deemed to be <i>fail-fast</i>.
 * This exception may be thrown even in single-threaded designs, if methods
 * are called out of order.
 *
 * @author J. Marrero
 */
class ConcurrentModificationException : public core::Exception
{
    AXF_EXCEPTION_TYPE(axf::collections::ConcurrentModificationException,
                       axf::core::Exception)

public:

    ConcurrentModificationException(const char* message);
    ~ConcurrentModificationException();
} ;

}
}

#endif /* CONCURRENTMODIFICATIONEXCEPTION_H */

