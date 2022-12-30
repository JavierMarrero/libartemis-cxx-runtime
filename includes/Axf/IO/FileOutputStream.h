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
 * File:   FileOutputStream.h
 * Author: Javier Marrero
 *
 * Created on December 28, 2022, 3:27 PM
 */

#ifndef FILEOUTPUTSTREAM_H
#define FILEOUTPUTSTREAM_H

// API
#include <Axf/IO/File.h>
#include <Axf/IO/OutputStream.h>

// C++
#include <cstdio>

namespace axf
{
namespace io
{

/**
 * A <code>FileOutputStream</code> is an output streams that handles writing
 * to <code>File</code> objects.
 * <p>
 * This stream carries an internal watermark that is updated on each write
 * operations. Only one output stream can be used to write into a file at any
 * time; subsequent trials of instancing a <code>FileOutputStream</code> with
 * the same <code>File</code> argument will fail with an exception.
 * <p>
 * This class also supports writing on different places within a file by the
 * means of a seek method. Standard C seek semantics is supported, with the
 * corresponding constants being declared as static members of this class.
 * <p>
 * This class is not designed to be inherited from.
 *
 * @return
 */
class FileOutputStream : public OutputStream
{
    AXF_CLASS_TYPE(axf::io::FileOutputStream,
                   AXF_TYPE(axf::io::OutputStream))
public:

    /**
     * Constructs a new output stream from a file.
     *
     * @param file
     */
    FileOutputStream(const File& file);

    /**
     * Default destructor of this class.
     */
    virtual ~FileOutputStream();

    virtual void close();

    virtual std::size_t write(const void* source, std::size_t bytes);

private:

    std::FILE*  m_f;            /// The file object pointer
    File        m_file;         /// The file object this stream encompasses
    std::size_t m_watermark;    /// The watermark

} ;

}
}

#endif /* FILEOUTPUTSTREAM_H */

