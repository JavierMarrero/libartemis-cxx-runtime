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
 * File:   file_test.cpp
 * Author: Javier Marrero
 *
 * Created on December 28, 2022, 4:11 PM
 */

#include <stdlib.h>
#include <iostream>

/*
 * Simple C++ Test Suite
 */
#include <Axf.h>

using namespace axf;
using namespace axf::io;

static void filestats(const File& f)
{
    std::cout << f.getAbsolutePath() << " exists? " << f.exists() << std::endl;
    std::cout << f.getAbsolutePath() << " is directory?: " << f.isDirectory() << std::endl;
}

int main(int argc, char** argv)
{
    File f("example.txt");
    File g(f, "example2.txt");
    File k(f, "example-dir");

    filestats(f);
    filestats(g);
    filestats(k);

    // Create
    std::cout << "creating the files..." << std::endl;
    if (f.exists() == false)
        f.create();
    if (g.exists() == false)
        g.create();

    std::cout << "testing writing..." << std::endl;
    {
        FileOutputStream stream(f);

        stream.writeString(core::string("Hello, world!\n"));
        stream.writeString(core::string("I'm a new file!...\n"));

        stream.close();
    }

    std::cout << "DONE..." << std::endl;
    std::getchar();

    // Remove
    std::cout << "removing the files..." << std::endl;
    if (f.exists())
        f.remove();
    if (g.exists())
        g.remove();

    return (EXIT_SUCCESS);
}

