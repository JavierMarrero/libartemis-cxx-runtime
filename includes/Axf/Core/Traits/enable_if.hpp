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
 * File:   enable_if.hpp
 * Author: Javier Marrero
 *
 * Created on August 17, 2022, 10:25 PM
 */

#ifndef ENABLE_IF_HPP
#define ENABLE_IF_HPP

namespace axf
{
namespace traits
{

template <typename T, typename R = void>
struct enable_if_has_type
{
    typedef R type;
} ;

template <bool B, class T = void>
struct enable_if_c
{
    typedef T type;
} ;

template <class T>
struct enable_if_c<false, T>
{
} ;

template <class Cond, class T = void>
struct enable_if : public enable_if_c<Cond::value, T>
{
} ;

}
}

#endif /* ENABLE_IF_HPP */

