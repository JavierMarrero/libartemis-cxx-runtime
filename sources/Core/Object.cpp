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
 * File:   Object.cpp
 * Author: Javier Marrero
 * 
 * Created on November 27, 2022, 1:18 AM
 */

#include <Axf/Core/Object.h>

// C
#include <cstring>

using namespace axf;
using namespace axf::core;

#define FNV_OFFSET_BASIS_32B    2166136261u
#define FNV_PRIME_32B           16777619u

const axf::core::Class<Object>& Object::getCompileTimeClass()
{
    static Class<Object> classVariable("axf::core::Object", NULL);

    // Return the statically allocated class object
    return classVariable;
}

Object::Object()
{
}

Object::~Object()
{
}

bool Object::equals(const Object& object) const
{
    return this == &object;
}

const bits::Type* Object::getRuntimeType() const
{
    return &getCompileTimeClass();
}

int Object::hashCode() const
{
    int hash = FNV_OFFSET_BASIS_32B;
    const char* memory = reinterpret_cast<const char*> (this);

    // TODO: replace sizeof with something more descriptive (use rtti)
    for (unsigned int i = 0; i < sizeof (*this); ++i)
    {
        char octet = *(memory + i);

        // FNV hashing
        hash ^= octet;
        hash *= FNV_PRIME_32B;
    }

    return hash;
}

string Object::toString() const
{
    char repr[127] = {0};
    std::sprintf(repr, "%s@%p", getClass<Object>().getName(), this);

    return string(repr);
}
