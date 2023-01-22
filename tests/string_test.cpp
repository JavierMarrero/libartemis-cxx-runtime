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
 * File:   string_test.cpp
 * Author: Javier Marrero
 *
 * Created on December 13, 2022, 1:26 PM
 */

#include <stdlib.h>
#include <iostream>

/*
 * Simple C++ Test Suite
 */
#include <Axf.h>

#define MESSAGE(x) std::cout << "\n--\n" << x << "\n--\n" << std::endl
#define TEST(x)     std::cout << "Testing " << x << "\n" << std::endl;
#define LINE(x)     std::cout << x << std::endl;

using axf::core::string;

int main(int argc, char** argv)
{
    {
        MESSAGE("Testing strings...");
        TEST("declarations and constructors");
        {
            string s1 = "normal ascii string...";
            string s2 = "normal utf8 encoded string... -- \xce\xba\xcf\x8c\xcf\x83\xce\xbc\xce\xbf\xcf\x82  --";
            string s3 = L"normal wide character string...";
            string w1 = L"äüëïö, áéíóú, -- control ascii sequence --";

            std::cout << s1 << std::endl;
            std::cout << s2 << std::endl;
            std::cout << s3 << std::endl;

            LINE("attempting to write UTF8 output to console: (may not work on windows)");
            std::cout << w1 << std::endl;

            LINE("attempting to write it as UTF16 encoded wchar_t");
            std::wcout << L"áéíóú, äëïöü, -- ascii --\n";
            std::wcout << w1.asWideString() << L"\n";

            LINE("copy constructor");
            string s4 = s1;

            LINE("before mutation");
            std::cout << s4 << std::endl;

            LINE("after mutation");
            std::cout << "s4: " << s4.append("*fragment appended*") << std::endl;
            std::cout << "s1: " << s1 << std::endl;
        }
        TEST("operations");
        {
            string s1 = "The quick brown fox jumps over the lazy dog.";
            string s2 = "How vexingly quick daft zebras jump!";

            TEST("append operation");

            std::cout << s1 + " " + s2 << std::endl;

            LINE("was the operation destructive?");
            std::cout << s1 << std::endl;
            std::cout << s2 << std::endl;

            TEST("equality comparison");
            {
                string s3 = "hello, world";
                string s4 = "hello, world";

                std::cout << "s1 == s2? " << s1.equals(s2) << std::endl;
                std::cout << "s3 == s4? " << (s3 == s4) << std::endl;
            }

            TEST("'at' operation");
            {
                LINE("control operation");
                std::wcout << L'á' << L" á" << L"\n";

                string utf8 = "\xc3\xa1 \xc3\xa9 \xc3\xad \xc3\xb3 \xc3\xba";

                std::cout << "s1[4] = " << s1.at(4) << std::endl; // must be a 'q'
                std::cout << "s1[2] = " << s1.at(2) << std::endl; // must be a 'e'
                std::cout << "s1[8] = " << s1.at(8) << std::endl; // must be a 'k'
                std::wcout << L"utf8[0] = " << utf8.at(0).toWideCharacter() << L"\n"; // must be 'á'
                std::wcout << L"utf8[6] = " << utf8.at(6).toWideCharacter() << L"\n"; // must be 'ó'

                wchar_t wc = *utf8.at(0).toWideCharacter();
                wchar_t wc2 = *utf8.at(6).toWideCharacter();

                if (wc == L'á' && wc2 == L'ó')
                {
                    std::cout << "all correct..." << std::endl;
                }
            }

            TEST("'substring' operation");
            {
                std::cout << "substring s1: " << s1.substring(4, 9) << std::endl;
            }
        }
    }
    std::cout << "done... press any key to continue..." << std::endl;
    std::getchar();
    return (EXIT_SUCCESS);
}

