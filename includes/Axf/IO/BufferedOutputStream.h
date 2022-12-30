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
 * File:   BufferedOutputStream.h
 * Author: Javier Marrero
 *
 * Created on December 28, 2022, 12:09 AM
 */

#ifndef BUFFEREDOUTPUTSTREAM_H
#define BUFFEREDOUTPUTSTREAM_H

// API
#include <Axf/Core/DynamicArray.h>
#include <Axf/IO/OutputStream.h>

namespace axf
{
namespace io
{

/**
 * A <code>BufferedOutputStream</code> is a type of output stream that contains
 * an internal buffer.
 * <p>
 * Whenever a write operation is performed on that buffer, the buffer is filled
 * up to its desired capacity. Once the buffer is full, or the stream is closed,
 * the contents are written to the underlying stream.
 * <p>
 * This type of buffered I/O allows for (probably) less system calls and a more
 * efficient use of the underlying streams. On streams that are already buffered
 * by definition, this may allow a double buffering, increasing bit rate in
 * data transfer.
 *
 * @return
 */
class BufferedOutputStream : public OutputStream
{
    AXF_CLASS_TYPE(axf::io::BufferedOutputStream, AXF_TYPE(axf::io::OutputStream))
public:

    BufferedOutputStream(OutputStream& stream, std::size_t size = 0x2000);
    virtual ~BufferedOutputStream();

    virtual void close();

    virtual std::size_t write(const void* source, std::size_t bytes);

protected:

    core::DynamicArray<char>    m_buffer;
    std::size_t                 m_count;
    OutputStream&               m_stream;

private:

    void flushBuffer();
} ;

}
}

#endif /* BUFFEREDOUTPUTSTREAM_H */

