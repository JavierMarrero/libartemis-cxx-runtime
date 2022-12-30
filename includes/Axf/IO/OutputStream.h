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
 * File:   OutputStream.h
 * Author: Javier Marrero
 *
 * Created on December 14, 2022, 10:31 PM
 */

#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

// API
#include <Axf/IO/IOStream.h>

// C
#include <cstddef>

namespace axf
{
namespace io
{

/**
 * The <code>OutputStream</code> interface handles the transfer of data from
 * a source to a destination. This operation may be translated into writing from
 * some source to a destination.
 * <p>
 * Streams allow great flexibility on I/O operations, since they decouple and
 * abstract many details of the arriving context; and allows to treat multiple
 * destinations homogeneously.
 * <p>
 * This class handles output alone, and may not be used for input receiving.
 * This class also implements the observer pattern, allowing to notify objects
 * on write events.
 *
 * @author J. Marrero
 */
class OutputStream : public IOStream
{

    AXF_CLASS_TYPE(axf::io::OutputStream,
                   AXF_TYPE(axf::io::IOStream))

public:

    /**
     * Default virtual destructor
     */
    virtual ~OutputStream() { }

    /**
     * Writes 'bytes' bytes into the destination buffer from the source buffer.
     * 
     * @param destination
     * @param source
     * @param bytes
     * @return
     */
    virtual std::size_t write(const void* source, std::size_t bytes) = 0;

    /**
     * Writes a string to this output stream. This is a convenience method.
     *
     * @param destination
     * @param str
     * @return
     */
    inline std::size_t writeString(const core::string& str)
    {
        return write(str.bytes(), str.size());
    }
} ;

}
}

#endif /* OUTPUTSTREAM_H */

