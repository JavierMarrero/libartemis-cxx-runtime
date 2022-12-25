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
 * File:   pointers.h
 * Author: Javier Marrero
 *
 * Created on December 23, 2022, 12:15 PM
 */

#ifndef ARTEMIS_POINTERS_H
#define ARTEMIS_POINTERS_H

namespace axf
{
namespace core
{

/**
 * Defines an aliasing relationship between two pointers without breaking
 * strict aliasing rule. The member 'alias' becomes an alias to the pointer
 * 'aliased'. Both are accessible through the union.
 *
 * @author J. Marrero
 */
template <typename T1, typename T2>
union pointer_alias
{
    T1* m_alias;
    T2* m_aliased;
} ;

}
}

#endif /* POINTERS_H */

