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
 * File:   hashset_test.cpp
 * Author: Javier Marrero
 *
 * Created on December 7, 2022, 5:07 PM
 */

#include <stdlib.h>
#include <iostream>

#include <Axf.h>

#include <vector>

/*
 * Simple C++ Test Suite
 */
using namespace axf;
using namespace axf::collections;

int main(int argc, char** argv)
{
    try
    {
        HashSet<int> intSet(2);

        // Add the numbers from zero to nine
        for (int i = 0; i < 10; ++i)
        {
            std::cout << "adding " <<  i << " to the set: " << intSet.add(i) << std::endl;
        }

        // Attempt to include a duplicate
        std::cout << "added duplicate 5?: " << intSet.add(5) << std::endl;

        // Contains?
        std::cout << "contains a 3?: " << intSet.contains(3) << std::endl;
        std::cout << "contains a 139?: " << intSet.contains(139) << std::endl;

        // Remove
        std::cout << "removed a 7?: " << intSet.remove(7) << std::endl;

        // Iterate over the set
        std::cout << "set: ";
        for (collections::iterator<int> it = intSet.begin(),
             end = intSet.end(); it != end; ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    catch (core::Exception& ex)
    {
        std::cerr << ex.getClassName() << ": " << ex.getMessage() << std::endl;
    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    // Pause
    std::cout << "done..." << std::endl;
    std::getchar();
    return (EXIT_SUCCESS);
}

