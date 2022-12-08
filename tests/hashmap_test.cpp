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
 * File:   hashmap_test.cpp
 * Author: Javier Marrero
 *
 * Created on December 6, 2022, 7:49 PM
 */

#include <stdlib.h>
#include <iostream>

/*
 * Simple C++ Test Suite
 */
#include <Axf.h>

using namespace axf;
using namespace axf::core;
using namespace axf::collections;

template <typename K, typename V>
void printMap(const Map<K, V>& map)
{

}

int main(int argc, char** argv)
{
    std::cout << "hash map creation " << std::endl;
    HashMap<int, char> hashMap;

    // Adds the 10 first letters to the map
    for (int i = 0; i < 10; ++i)
    {
        hashMap.put(i, 'a' + i);
    }

    // Print the state of the map
    printMap(hashMap);

    // Remove

    // Print the state of the map
    printMap(hashMap);

    std::cout << "done..." << std::endl;
    std::getchar();
    return (EXIT_SUCCESS);
}

