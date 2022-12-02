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
 * File:   String.cpp
 * Author: Javier Marrero
 * 
 * Created on November 27, 2022, 6:11 PM
 */

#include <Axf/Core/String.h>
#include <Axf/Core/IndexOutOfBoundsException.h>
#include <Axf/Core/IllegalStateException.h>

using namespace axf;
using namespace axf::core;

string::string()
:
m_buffer(NULL),
m_capacity(0),
m_size(0)
{
}

string::string(const char* cstr)
:
m_buffer(NULL),
m_capacity(0),
m_size(0)
{
}

string::string(const wchar_t* wstr)
:
m_buffer(NULL),
m_capacity(0),
m_size(0)
{
}

string::~string()
{
    clear();
}

string& string::append(const string& str)
{
    return *this;
}

string::utf8_char* string::arrayCopy(int startIndex, int endIndex, utf8_char* newArray) const
{
    if (startIndex < 0)
        startIndex = 0;
    if (endIndex >= (int) m_size)
        endIndex = m_size;

    for (int i = startIndex; i < endIndex; ++i)
    {
        newArray[i] = m_buffer[i];
    }

    return newArray;
}

void string::checkIndexExclusive(int index)
{
    if (index < 0 || index >= ((int) m_size))
    {
        throw IndexOutOfBoundsException("the index is not between 0 and size.", index);
    }
}

void string::clear()
{
    if (m_buffer != NULL)
    {
        delete m_buffer;
    }
    m_capacity = 0;
    m_length = 0;
    m_size = 0;
}

void string::resize(int delta)
{
    if (((int) m_capacity) - delta < 0)
        throw IllegalStateException("attempted to resize a string below zero capacity.");

}
