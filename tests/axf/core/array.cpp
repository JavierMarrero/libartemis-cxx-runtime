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
 * File:   array.cpp
 * Author: Javier Marrero
 *
 * Created on November 28, 2022, 8:30 PM
 */

#include <stdlib.h>
#include <iostream>

#include <Axf.h>

using namespace axf;
using namespace axf::core;

int main(int argc, char** argv)
{
    Array<int, 10> array;
    for (int i = 0; i < 10; ++i)
    {
        array[i] = i;
    }

    size_t j = array.length;
    while (j-- > 0)
    {
        std::cout << array[j] << ", ";
    }
    std::cout << std::endl;

    
    std::getchar();
    return (EXIT_SUCCESS);
}

