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
 * File:   eh_test.cpp
 * Author: Javier Marrero
 *
 * Created on December 11, 2022, 8:19 PM
 */

#include <stdlib.h>
#include <iostream>

#include <Axf.h>

using namespace axf;
using namespace axf::core;

int main(int argc, char** argv)
{
    // Caught exception
    try
    {
        throw IllegalStateException("this exception must be caught, don't be a jerk.");
    }
    catch (Exception& ex)
    {
        std::cout << "caught exception: " << ex.getClassName() << ": " << ex.getMessage() << std::endl;
    }

    // Uncaught exception
    try
    {
        throw IllegalStateException("bogus...");
    }
    catch (Exception& ex)
    {
        throw IllegalStateException("uncaught exception thrown on purpose!");
    }
    return (EXIT_SUCCESS);
}

