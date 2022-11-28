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
 * File:   smart_references.cpp
 * Author: Javier Marrero
 *
 * Created on November 28, 2022, 5:02 PM
 */

#include <stdlib.h>
#include <iostream>
#include <cstdio>

#include <Axf.h>

class Dummy
{
public:

    Dummy()
    {
        std::cout << "creating a new dummy at " << this << std::endl;
    }

    ~Dummy()
    {
        std::cout << "deleting a dummy at " << this << std::endl;
    }
} ;

void test_unique_ptr()
{
    axf::core::unique_ref<Dummy> dummy = new Dummy();
}

int main(int argc, char** argv)
{
    std::printf("testing unique pointers...\n");
    test_unique_ptr();

    std::getchar();
    return (EXIT_SUCCESS);
}

