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
 * File:   is_base_of.hpp
 * Author: Javier Marrero
 *
 * Created on August 17, 2022, 9:42 PM
 */

#ifndef IS_BASE_OF_HPP
#define IS_BASE_OF_HPP

/* Selenium API */
#include "is_base_and_derived.hpp"
#include "remove_cv.hpp"

namespace axf
{
namespace traits
{
namespace bits
{

template <class B, class D>
struct is_base_of_impl
{
    typedef typename remove_cv<B>::type ncvB;
    typedef typename remove_cv<D>::type ncvD;

    ///TODO: Being a class is also a requisite
    static const bool value = (ARTEMIS_IS_BASE_OF(ncvB, ncvD) || is_same<ncvB, ncvD>::value);
} ;

}

template <class Base, class Derived> struct is_base_of
: public integral_constant<bool, (bits::is_base_of_impl<Base, Derived>::value)>
{
} ;

}
}

#endif /* IS_BASE_OF_HPP */

