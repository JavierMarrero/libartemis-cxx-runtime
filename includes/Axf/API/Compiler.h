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
 * File:   Compiler.h
 * Author: Javier Marrero
 *
 * Created on August 15, 2022, 12:47 AM
 */

#ifndef COMPILER_H
#define COMPILER_H

#if defined(__GNUC__)
#define ARTEMIS_COMPILER_GCC_COMPATIBLE 1
#define GNUC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)
#endif

#if defined(__GNUC__) && !defined(__llvm__) && !defined(__INTEL_COMPILER)
#define ARTEMIS_COMPILER_GCC   __GNUC__ // probably
#endif

/* Configurations for GCC only */

/* Language-wide configuration */
#if __cplusplus >= 201103L
#define ARTEMIS_CXX11_SUPPORTED    1
#endif

#endif /* COMPILER_H */

