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

#include <Axf/Core/Class.h>

// C++
#include <cstring>

using namespace axf;
using namespace axf::core;
using namespace axf::core::bits;

int bits::Type::encodeTypeName(const char* str)
{
    size_t len = std::strlen(str);
    int hash = 0;

    for (unsigned i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

const char* Type::getSimpleName() const
{
    const char* result = m_className;

    size_t i = std::strlen(m_className);
    while ((i >= 0) && (*(result + (i - 1)) != ':'))
    {
        --i;
    }

    return result + i;
}
