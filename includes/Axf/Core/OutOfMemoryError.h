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
 * File:   OutOfMemoryError.h
 * Author: Javier Marrero
 *
 * Created on December 4, 2022, 12:20 PM
 */

#ifndef OUTOFMEMORYERROR_H
#define OUTOFMEMORYERROR_H

// API
#include <Axf/Core/Exception.h>

// C
#include <climits>

namespace axf
{
namespace core
{

/**
 * Signals the runtime system that allocation requests are no longer available
 * due to a shortage in system's available memory.
 * <p>
 * 
 *
 * @return
 */
class OutOfMemoryError : public Exception
{
    AXF_EXCEPTION_TYPE(axf::core::OutOfMemoryError, axf::core::Exception)

public:

    OutOfMemoryError();
    OutOfMemoryError(const char* message, std::size_t requested = 0);
    ~OutOfMemoryError();

    /**
     * Returns how many bytes were requested upon allocation that caused the
     * memory shortage.
     *
     * @return
     */
    inline std::size_t getRequested() const
    {
        return m_requested;
    }

private:

    std::size_t m_requested;
} ;

}
}

#endif /* OUTOFMEMORYERROR_H */

