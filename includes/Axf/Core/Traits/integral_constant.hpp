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
 * File:   integral_constant.hpp
 * Author: Javier Marrero
 *
 * Created on August 17, 2022, 9:32 PM
 */

#ifndef INTEGRAL_CONSTANT_HPP
#define INTEGRAL_CONSTANT_HPP

namespace axf
{
namespace traits
{

template <typename T, T val>
struct integral_constant
{
    typedef T value_type;
    typedef integral_constant<T, val> type;

    static const T value = val;
} ;

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

}
}

#endif /* INTEGRAL_CONSTANT_HPP */

