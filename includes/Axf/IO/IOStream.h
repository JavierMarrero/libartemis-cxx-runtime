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
 * File:   IOStream.h
 * Author: Javier Marrero
 *
 * Created on December 28, 2022, 12:05 PM
 */

#ifndef IOSTREAM_H
#define IOSTREAM_H

// API
#include <Axf/Core/Object.h>
#include <Axf/IO/Closeable.h>

namespace axf
{
namespace io
{

/**
 * The <code>IOStream</code> interface groups all the common stream
 * functionality.
 * <p>
 * The different output and input streams implements the functionality declared
 * in this interface.
 * 
 * @author J. Marrero
 */
class IOStream : public io::Closeable
{

    AXF_CLASS_TYPE(axf::io::IOStream, AXF_TYPE(axf::io::Closeable))
public:

    /**
     * Default destructor. Does nothing as <code>IOStream</code> is not
     * prone to be instanced.
     */
    virtual ~IOStream() { }

} ;

}
}

#endif /* IOSTREAM_H */

