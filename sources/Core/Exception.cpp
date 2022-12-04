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
 * File:   Exception.cpp
 * Author: Javier Marrero
 * 
 * Created on November 27, 2022, 4:48 PM
 */

#include <Axf/Core/Exception.h>
#include <Axf/Core/IllegalStateException.h>

// C
#include <cstring>

using namespace axf;
using namespace axf::core;
using namespace axf::core::bits;

ExceptionTypeDescriptor::ExceptionTypeDescriptor(const char* className, const ExceptionTypeDescriptor* super)
:
m_className(className),
m_super(super)
{
}

ExceptionTypeDescriptor::~ExceptionTypeDescriptor()
{
}

bool ExceptionTypeDescriptor::isInstanceOf(const ExceptionTypeDescriptor& exceptionType) const
{
    return this == &exceptionType;
}

bool ExceptionTypeDescriptor::isKindOf(const ExceptionTypeDescriptor& exceptionType) const
{
    const ExceptionTypeDescriptor* current = this;
    while (current != NULL)
    {
        if (current == &exceptionType)
        {
            return true;
        }
        current = current->m_super;
    }
    return false;
}

const ExceptionTypeDescriptor& ExceptionTypeDescriptor::super() const
{
    if (m_super == NULL)
        throw IllegalStateException("attempted to retrieve the super-type of a base class!");
    return *m_super;
}

const ExceptionTypeDescriptor& Exception::getCompileTimeClass()
{
    static ExceptionTypeDescriptor descriptor("axf::core::Exception", NULL);

    // Return a reference to the newly allocated constructor
    return descriptor;
}

Exception::Exception(const char* message)
{
    std::strncpy(m_message, message, 1024);
}

Exception::~Exception()
{
}

const bits::ExceptionTypeDescriptor& Exception::getClass() const
{
    return getCompileTimeClass();
}

const char* Exception::getClassName() const
{
    return "axf::core::Exception";
}
