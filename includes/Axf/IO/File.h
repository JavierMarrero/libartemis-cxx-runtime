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
 * File:   File.h
 * Author: Javier Marrero
 *
 * Created on December 27, 2022, 5:53 PM
 */

#ifndef FILE_H
#define FILE_H

// API
#include <Axf/Collections/List.h>
#include <Axf/Core/Object.h>
#include <Axf/Core/Memory.h>

namespace axf
{
namespace io
{

/**
 * The <code>File</code> class represents an abstract pathname in a file system.
 * <p>
 * Objects of this class represents pathnames in a concrete machine's file system.
 * This provides an operating system independent abstraction to manipulate files
 * and file-like objects.
 * <p>
 * I/O on the <i>Artemis Framework</i> is not directed towards file objects;
 * it is rather directed to streams. Streams provide abstractions of I/O
 * that are independent of the underlying media.
 * <p>
 * Instead, this class is used to represent only file like objects in a concrete
 * file system.
 * <p>
 * This class is designed to be as lightweight as possible, so that it may be
 * copied by value. It includes a shared reference to a file resource, and
 * when no more references are being held to the file handle, the file gets
 * automatically closed.
 * <p>
 * This class also support binary writing and binary reading, but the methods
 * are held private. Use the <code>FileInputStream</code> and
 * <code>FileOutputStream</code> classes instead; which this class declares
 * as friends.
 * <p>
 * This class also provides a built-in replacement for all the "stdio.h" functions
 * that operates on files; as well as some POSIX compatible routines.
 *
 * @author J. Marrero
 */
class File : public core::Object
{
    AXF_CLASS_TYPE(axf::io::File, AXF_TYPE(axf::core::Object))

    // Friend classes
    friend class FileOutputStream;
    friend class FileInputStream;

public:

    static const core::uchar PATH_SEPARATOR;

    /**
     * Creates a new object that describes the given abstract pathname.
     *
     * @param name
     */
    File(const core::string& name);

    /**
     * Creates a new file from a parent file.
     *
     * @param parent
     * @param name
     */
    File(const File& parent, const core::string& name);

    /**
     * Default copy constructor.
     */
    File(const File& rhs);

    /**
     * Destructs the object.
     */
    virtual ~File();

    /**
     * Creates a new file if this file does not exists. It does not have any
     * effect if the file already exists. If the file is a directory, it
     * creates a file with analogous name to the directory.
     * <p>
     * This method may throw a IOException if the file couldn't be created.
     */
    void create() const;

    /**
     * Returns true if the pathname defined by this <code>File</code> object
     * exists.
     *
     * @return
     */
    bool exists() const;

    /**
     * Returns the absolute pathname of this file.
     *
     * @return
     */
    inline const core::string& getAbsolutePath() const
    {
        return m_name;
    }

    /**
     * Returns the parent file of this file. If this file has no parent, throws
     * an exception of type <code>IOException</code>.
     *
     * @return
     */
    File getParentFile() const;

    /**
     * Returns true if the abstract path name this file describes is actually
     * a directory in the file system.
     *
     * @return
     */
    bool isDirectory() const;

    /**
     * Enumerates all the files that belongs to this pathname (the pathname must
     * be a valid directory) and returns a list of these file objects.
     *
     * @return
     */
    core::strong_ref<collections::List<File> > listAllFiles() const;

    /**
     * Creates the last immediate subdirectory that corresponds to this path.
     * This function will return false if it fails to create the directory.
     * This may happen under the following circumstances.
     * <ul>
     *  <li>Any of the intermediate subdirectories to reach this path does not exists.</li>
     *  <li>There was a file system exception that prevented the creation of the directory.</li>
     * </ul>
     *
     * @return a boolean indicating success or failure.
     */
    bool mkdir() const;

    /**
     * Removes the pathname pointed by this <code>File</code> object. This
     * handles both file and directory removal.
     * <p>
     * If the operation fail, it shall return <code>false</code>, and return
     * without further notice. No exceptions are thrown by this method whatsoever.
     * 
     * @return
     */
    bool remove() const noexcept;

private:

    /**
     * Converts a relative pathname to an absolute one. Works with Unicode
     * encoded file names.
     *
     * @param path
     * @return
     */
    static core::string relativeToAbsolutePath(const core::string& path);

    std::size_t     m_filePointer;  /// The file pointer
    core::string    m_name;         /// The name that identifies this abstract pathname

    /**
     * Removes from the file system the directory described by this abstract path
     * name. As this function is private, no invalid input data is assumed,
     * therefore, no security checks are performed.
     * <p>
     * This function may also construct some new file objects recursively,
     * however, these will be stack allocated and must not consume lots of
     * computational resources.
     * <p>
     * By removing a directory, all the 
     *
     */
    bool removeDirectory() const;

    /**
     * Reads a determinate number of bytes from a file and places them in the
     * specified buffer.
     *
     * @param buffer
     * @param length
     * @return
     */
    std::size_t read(void* buffer, std::size_t length);

    /**
     * Moves the file pointer forward or backwards.
     *
     * @param offset
     * @param current
     */
    inline void seek(std::size_t offset, int current);

} ;

}
}

#endif /* FILE_H */

