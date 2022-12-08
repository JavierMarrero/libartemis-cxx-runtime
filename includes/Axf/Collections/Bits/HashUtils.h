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
 * File:   HashUtils.h
 * Author: Javier Marrero
 *
 * Created on December 7, 2022, 5:20 PM
 */

#ifndef HASHUTILS_H
#define HASHUTILS_H

namespace axf
{
namespace collections
{
namespace __
{

/**
 * This structure represents a default hash functor. The hash used is a variant
 * of the sbdm algorithm that produces a 32bit hash.
 */
template <typename T>
struct default_hash
{

    unsigned int operator()(const T& element) const
    {
        const char* memory = reinterpret_cast<const char*> (&element);
        unsigned int hash = 0;

        for (unsigned i = 0; i < sizeof (T); ++i)
        {
            char c = memory[i];
            hash = c + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
    }
} ;

}
}
}

#endif /* HASHUTILS_H */

