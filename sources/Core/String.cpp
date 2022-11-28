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

using namespace axf;
using namespace axf::core;

String::String()
:
m_buffer(NULL),
m_capacity(0),
m_size(0)
{
}

String::~String()
{
    clear();
}

String::String(const char* cstr)
{
}

void String::clear()
{
    if (m_buffer != NULL)
    {
        delete m_buffer;
    }
    m_capacity = 0;
    m_size = 0;
}
