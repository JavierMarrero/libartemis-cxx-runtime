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
 * File:   BufferedOutputStream.cpp
 * Author: Javier Marrero
 * 
 * Created on December 28, 2022, 12:09 AM
 */

#include <Axf/IO/BufferedOutputStream.h>
#include <Axf/Core/Runtime.h>

// C++
#include <cstring>

using namespace axf;
using namespace axf::io;

BufferedOutputStream::BufferedOutputStream(OutputStream& stream, std::size_t size)
:
m_buffer(size),
m_count(0),
m_stream(stream)
{
}

BufferedOutputStream::~BufferedOutputStream()
{
}

void BufferedOutputStream::close()
{
    m_stream.close();
}

void BufferedOutputStream::flushBuffer()
{
    if (m_count > 0)
    {
        std::memset(m_buffer, 0, m_count);
        m_count = 0;
    }
}

std::size_t BufferedOutputStream::write(const void* source, std::size_t bytes)
{
    if (bytes >= m_buffer.length())
    {
        flushBuffer();
        return m_stream.write(source, bytes);
    }

    if (bytes > (m_buffer.length() - m_count))
    {
        flushBuffer();
    }

    core::arrayCopy(m_buffer.decay(), m_count, static_cast<const char*> (source), 0, bytes);
    m_count += bytes;
    return bytes;
}




