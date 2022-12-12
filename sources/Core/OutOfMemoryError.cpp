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
 * File:   OutOfMemoryError.cpp
 * Author: Javier Marrero
 * 
 * Created on December 4, 2022, 12:20 PM
 */

#include <Axf/Core/OutOfMemoryError.h>

using namespace axf;
using namespace axf::core;

OutOfMemoryError::OutOfMemoryError()
:
Exception("the system has ran out of usable memory!"),
m_requested(0)
{
}

OutOfMemoryError::OutOfMemoryError(const char* message, std::size_t requested)
:
Exception(message),
m_requested(requested)
{
}

OutOfMemoryError::~OutOfMemoryError()
{
}


