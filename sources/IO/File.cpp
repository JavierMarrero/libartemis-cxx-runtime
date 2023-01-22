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
 * File:   File.cpp
 * Author: Javier Marrero
 * 
 * Created on December 27, 2022, 5:53 PM
 */

#include <Axf/IO/File.h>

#include <Axf/API/Platform.h>
#include <Axf/Collections/ArrayList.h>
#include <Axf/IO/FileNotFoundException.h>

// C++
#include <cstdio>
#include <cwchar>
#include <cerrno>

#ifdef ARTEMIS_PLATFORM_W32
#    include <windows.h>
#endif

#ifdef ARTEMIS_POSIX
#    include <unistd.h>
#    include <dirent.h>
#endif

using namespace axf;
using namespace axf::core;
using namespace axf::collections;
using namespace axf::io;

#ifdef ARTEMIS_PLATFORM_W32
const core::uchar File::PATH_SEPARATOR('\\');
#else
const core::uchar File::PATH_SEPARATOR('/');
#endif

core::string File::relativeToAbsolutePath(const core::string& path)
{
#ifdef ARTEMIS_PLATFORM_W32
    wchar_t buffer[1024] = {0};
    GetFullPathNameW(path.asWideString(), 1024, buffer, NULL);

    return buffer;
#endif
}

File::File(const core::string& name)
:
m_filePointer(0),
m_name(relativeToAbsolutePath(name))
{
}

File::File(const File& parent, const core::string& name)
:
m_filePointer(0)
{
    core::string path = parent.m_name;
    if (parent.isDirectory() == false)
    {
        path = parent.getParentFile().m_name;
    }
    m_name = path.append(PATH_SEPARATOR).append(name);
}

File::File(const File& rhs)
:
m_filePointer(rhs.m_filePointer),
m_name(rhs.m_name)
{
}

File::~File()
{
}

void File::create() const
{
    if (exists() == false)
    {
        const wchar_t* wname = m_name.asWideString();
        std::FILE* f = _wfopen(wname, L"w");
        if (f == nullptr)
        {
            throw IOException("unable to create file");
        }

        fclose(f);
    }
}

bool File::exists() const
{
#ifdef ARTEMIS_PLATFORM_W32
    return ((INVALID_FILE_ATTRIBUTES == GetFileAttributesW(m_name.asWideString())) && (GetLastError() == ERROR_FILE_NOT_FOUND)) == false;
#endif
}

File File::getParentFile() const
{
    std::size_t lastPathSeparator = m_name.lastIndexOf(PATH_SEPARATOR);
    if (lastPathSeparator == core::string::NPOS)
        throw IOException("attempted to get parent file to file with no parent.");

    return File(m_name.substring(0, lastPathSeparator));
}

bool File::isDirectory() const
{
#ifdef ARTEMIS_PLATFORM_W32
    DWORD attributes = GetFileAttributesW(m_name.asWideString());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return false;

    return ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#endif
}

core::strong_ref<collections::List<File> > File::listAllFiles() const
{
    collections::ArrayList<File>* files = new collections::ArrayList<File>();

#ifdef ARTEMIS_PLATFORM_W32
    HANDLE dir;
    WIN32_FIND_DATAW fileData;

    if ((dir = FindFirstFileW((m_name + "/*").asWideString(), &fileData)) == INVALID_HANDLE_VALUE)
        return files; /* No files found */

    do
    {
        const string fileName = fileData.cFileName;
        //        const string fullFileName = m_name + "/" + fileName;
        //        const bool is_directory = (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (fileName[0] == '.')
            continue;

        //        if (is_directory)
        //            continue;

        files->add(File(*this, fileName));
    }
    while (FindNextFileW(dir, &fileData));

    FindClose(dir);
#endif

    return files;
}

bool File::mkdir() const
{
    if (exists() == false && isDirectory() == false)
    {
#ifdef ARTEMIS_PLATFORM_W32
        return (CreateDirectoryW(m_name.asWideString(), NULL));
#endif
    }
    return false;
}

bool File::remove() const noexcept
{
    if (!exists()) return false;

    if (isDirectory())
    {
        return removeDirectory();
    }
    else
    {
        return _wremove(m_name.asWideString()) == 0;
    }
}

bool File::removeDirectory() const
{
    // List all the directories
    if (!isDirectory() || !exists())
    {
        return false;
    }

    strong_ref<List<File> > files = listAllFiles();
    for (iterator<File> it = files->begin(), end = files->end();
         it != end; it++)
    {
        (*it).remove();
    }

    // Finally, remove the directory
#ifdef ARTEMIS_PLATFORM_W32
    return (RemoveDirectoryW(m_name.asWideString()));
#endif
}

std::size_t File::read(void* buffer, std::size_t length)
{
    if (!exists())
        throw FileNotFoundException(*this, "unable to open file in read mode.");

    std::FILE* f = _wfopen(m_name.asWideString(), L"r+b");
    if (f == nullptr)
    {
        throw IOException("unable to open file for writing.");
    }

    std::fseek(f, m_filePointer, SEEK_SET);
    std::size_t result = std::fread(buffer, 1u, length, f);
    if (result == 0)
        throw IOException("error on file read");

    m_filePointer += result;
    std::fclose(f);
    return result;
}

void File::seek(std::size_t offset, int current)
{
    switch (current)
    {
        case SEEK_CUR:
            m_filePointer += offset;
            break;
        case SEEK_END:
            m_filePointer -= offset;
            break;
        case SEEK_SET:
            m_filePointer = offset;
            break;
    }
}


