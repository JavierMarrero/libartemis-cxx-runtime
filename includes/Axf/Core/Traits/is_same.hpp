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
 * File:   is_same.hpp
 * Author: Javier Marrero
 *
 * Created on August 17, 2022, 9:42 PM
 */

#ifndef IS_SAME_HPP
#define IS_SAME_HPP

#include "integral_constant.hpp"

namespace axf
{
namespace traits
{

template <class T, class U> struct is_same : public false_type {};
template <class T> struct is_same<T, T> : public true_type {};

}
}


#endif /* IS_SAME_HPP */

