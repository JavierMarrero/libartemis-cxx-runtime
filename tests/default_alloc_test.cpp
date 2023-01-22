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
 * File:   default_alloc_test.cpp
 * Author: Javier Marrero
 *
 * Created on December 9, 2022, 11:01 AM
 */

#include <stdlib.h>
#include <iostream>

/*
 * Simple C++ Test Suite
 */
#include <Axf.h>

using namespace axf;

struct alloc_t
{

    alloc_t()
    {
        std::cout << "creating test!" << std::endl;
    }

    virtual ~alloc_t()
    {
        std::cout << "deleting test!" << std::endl;
    }

    void sayHi(int i)
    {
        std::cout << "hi from object " << i << "!" << std::endl;
    }
} ;

static void print_pointer(int* pointer)
{
    std::cout << "allocated int: " << *pointer << " at " << pointer << std::endl;
}

int main(int argc, char** argv)
{
    // Testing with ints
    std::cout << "test int allocation and deallocation..." << std::endl;
    {
        collections::DefaultAllocator<int> allocator;

        int* i1 = allocator.newObject(5);
        int* i2 = allocator.newObject(19);

        int* array = allocator.allocate(250);

        print_pointer(i1);
        print_pointer(i2);

        for (int i = 0; i < 10; ++i)
        {
            array[i] = i + 1;
        }

        for (int i = 0; i < 10; ++i)
        {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;

        // delete the pointers
        allocator.deleteObject(i1);
        allocator.deleteObject(i2);

        allocator.deallocate(array);
    }
    // Basic functionality test over!
    std::cout << "test the rest of the functionality" << std::endl;
    {
        collections::DefaultAllocator<alloc_t> allocator;

        // Allocate a single object
        std::cout << "creating a single object..." << std::endl;
        alloc_t* object = allocator.newObject(alloc_t());

        // Deallocate the object
        allocator.deallocate(object);

        // Allocate an array
        std::cout << "creating an array of 10 elements" << std::endl;
        alloc_t* array = allocator.newArray(10);

        // Say hi at random spots
        for (int i = 0; i < 5; i++)
        {
            array[std::rand() % 10].sayHi(i);
        }

        // Deallocate an array
        std::cout << "deallocating the array..." << std::endl;
        allocator.deleteArray(array, 10);
    }
    // Pause
    std::cout << "finished..." << std::endl;
    std::getchar();
    return (EXIT_SUCCESS);
}

