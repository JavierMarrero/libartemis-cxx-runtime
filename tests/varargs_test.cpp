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
 * File:   varargs_test.cpp
 * Author: Javier Marrero
 *
 * Created on December 22, 2022, 8:29 PM
 */

#include <stdlib.h>
#include <iostream>

#include <Axf.h>

using namespace axf::core;

int sum(const varargs<int>& integers)
{
    std::cout << "accumulating " << integers.length() << " numbers" << std::endl;
    int sum = 0;
    for (std::size_t i = 0; i < integers.length(); ++i)
    {
        sum += integers[i];
    }
    return sum;
}

float multipleSum(const varargs<int>& values, const varargs<float>& weights)
{
    float result = 0;
    for (std::size_t i = 0; i < values.length(); ++i)
    {
        result += (values[i] * weights[i]);
    }
    return result;
}

int main(int argc, char** argv)
{
    int s = sum((make_varargs(1), 2, 3, 4, 5));
    float ms = multipleSum((make_varargs(1), 2, 3, 4, 5, 6, 7, 8, 9, 10),
                           (make_varargs(2.0f), 1.2f, 3.5f, 2.3f, 2.4f, 1.2f, 3.5f, 2.3f, 2.4f, 7.5f, 0.2f));

    std::cout << "'sum' -> " << s << std::endl;
    std::cout << "'multipleSum' -> " << ms << std::endl;

    std::cout << "check for va-list grow" << std::endl;
    s = sum((varargs<int>(1), 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
             16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
             33, 34, 35, 36, 37, 38, 39, 40));

    std::cout << "sum [ 1 -> 40 ] -> " << s << std::endl;

    std::getchar();
    return (EXIT_SUCCESS);
}

