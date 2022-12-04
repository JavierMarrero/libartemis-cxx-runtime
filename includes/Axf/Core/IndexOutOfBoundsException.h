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
 * File:   IndexOutOfBoundsException.h
 * Author: Javier Marrero
 *
 * Created on November 28, 2022, 7:58 PM
 */

#ifndef INDEXOUTOFBOUNDSEXCEPTION_H
#define INDEXOUTOFBOUNDSEXCEPTION_H

// API
#include <Axf/Core/Exception.h>

namespace axf
{
namespace core
{

/**
 * An exception that is thrown whenever an invalid number is used as index.
 * <p>
 * These exceptions show commonly when dealing with arrays or strings. Whenever
 * an operation expects an index and an invalid integer is supplied, this
 * exception raises. This exception keeps the misused integer as a data field
 * for subsequent retrieval.
 * <p>
 * However, primitive array operations will still 'segfault' instead of
 * throwing an exception. This is default C/C++ behavior and it is impossible
 * to change. One may prefer to use, when exception safety is needed, the
 * Array class, that provides type-safe operations while keeping a very low
 * overhead.
 *
 * @author J. Marrero
 */
class IndexOutOfBoundsException : public Exception
{
    AXF_EXCEPTION_TYPE(axf::core::IndexOutOfBoundsException, axf::core::Exception)

public:

    IndexOutOfBoundsException(const char* message, long long index);
    ~IndexOutOfBoundsException();

    /**
     * Returns the offending index.
     * 
     * @return
     */
    inline long long getIndex() const
    {
        return m_index;
    }

private:

    long long m_index;  /// The index data field
} ;

}
}

#endif /* INDEXOUTOFBOUNDSEXCEPTION_H */

