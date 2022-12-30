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
 * File:   FileOutputStream.cpp
 * Author: Javier Marrero
 * 
 * Created on December 28, 2022, 3:27 PM
 */

#include <Axf/IO/FileOutputStream.h>
#include <Axf/IO/IOException.h>
#include <Axf/IO/FileNotFoundException.h>

// C++
#include <cstdio>
#include <cwchar>

using namespace axf;
using namespace axf::io;

FileOutputStream::FileOutputStream(const File& file)
:
m_file(file),
m_watermark(0)
{
    if (!m_file.exists())
        throw FileNotFoundException(m_file, "unable to open file in write mode.");

    m_f = _wfopen(m_file.getAbsolutePath().asWideString(), L"r+b");
    if (m_f == nullptr)
    {
        throw IOException("unable to open file for writing.");
    }
}

FileOutputStream::~FileOutputStream()
{
}

void FileOutputStream::close()
{
    std::fclose(m_f);
}

std::size_t FileOutputStream::write(const void* source, std::size_t bytes)
{
    std::fseek(m_f, m_watermark, SEEK_SET);
    std::size_t result = std::fwrite(source, 1u, bytes, m_f);
    if (result == 0)
        throw IOException("error on file write");

    m_watermark += result;
    return 0;
}

