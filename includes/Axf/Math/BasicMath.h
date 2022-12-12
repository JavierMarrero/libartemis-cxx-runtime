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
 * File:   BasicMath.h
 * Author: Javier Marrero
 *
 * Created on December 9, 2022, 3:10 PM
 */

#ifndef BASICMATH_H
#define BASICMATH_H

namespace axf
{
namespace math
{

/**
 * Returns the maximum between two elements. The elements must be comparable
 * using operator <code>></code>.
 *
 * @param a
 * @param b
 * @return a or b, the greater
 */
template <typename T>
T max(const T a, const T b)
{
    return (a > b) ? a : b;
}

/**
 * Returns the minimum between two elements. The elements must be comparable
 * using operator <code>></code>
 *
 * @param a
 * @param b
 * @return a or b, the lesser
 */
template <typename T>
T min(const T a, const T b)
{
    return (a > b) ? b : a;
}

}
}

#endif /* BASICMATH_H */

