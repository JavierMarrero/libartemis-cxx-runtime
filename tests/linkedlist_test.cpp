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
 * File:   linkedlist_test.cpp
 * Author: Javier Marrero
 *
 * Created on December 4, 2022, 12:49 PM
 */

#include <stdlib.h>
#include <iostream>

#include <Axf.h>

using namespace axf;

int main(int argc, char** argv)
{
    collections::LinkedList<int> linkedList;
    for (int i = 0; i < 15; ++i)
    {
        linkedList.add(i);
    }

    // add with index
    for (int i = 0; i < 6; ++i)
    {
        linkedList.add((2 * i) + 1, 55);
    }

    // Print the list before removing
    std::cout << "list: ";
    for (unsigned i = 0; i < linkedList.size(); ++i)
    {
        std::cout << linkedList.get(i) << " ";
    }
    std::cout << std::endl;

    // remove the 3 first elements
    for (int i = 0; i < 3; ++i)
    {
        linkedList.removeAt(0);
    }

    // remove
    std::cout << "removed 7?: " << linkedList.remove(7) << std::endl;
    std::cout << "removed 156?: " << linkedList.remove(156) << std::endl;

    // Test for iterators
    std::cout << "list: ";
    for (collections::iterator_ref<int> it = linkedList.begin(),
         end = linkedList.end(); it != end; it->next())
    {
        std::cout << **it << " ";
    }
    std::cout << std::endl;

    std::getchar();
    return (EXIT_SUCCESS);
}

