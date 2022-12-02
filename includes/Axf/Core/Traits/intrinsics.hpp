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
 * File:   intrinsics.hpp
 * Author: Javier Marrero
 *
 * Created on August 17, 2022, 9:29 PM
 */

#ifndef INTRINSICS_HPP
#define INTRINSICS_HPP

/* Configurations */
#include <Axf/API/Compiler.h>

#if defined(ARTEMIS_COMPILER_GCC)
#   if GNUC_VERSION >= 40403
#       define ARTEMIS_IS_UNION(T)         __is_union(T)
#       define ARTEMIS_IS_CLASS(T)         __is_class(T)
#       define ARTEMIS_IS_BASE_OF(T, U)    (__is_base_of(T, U) && !axf::traits::is_same<T, U>::value)
#   endif
#endif

#endif /* INTRINSICS_HPP */

