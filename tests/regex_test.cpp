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
 * File:   regex_test.cpp
 * Author: Javier Marrero
 *
 * Created on December 23, 2022, 5:22 PM
 */

#include <stdlib.h>
#include <iostream>

#include <Axf.h>

using namespace axf;
using namespace axf::core;
using namespace axf::text::regex;

int main(int argc, char** argv)
{
    std::cout << "regular expressions test" << std::endl;
    {
        Pattern pattern;
        pattern.declareStates((make_varargs(string("q0")), "q1", "q2", "q3"));
        pattern.setInitialState(pattern.getState("q0"));
        pattern.setFinalStates((make_varargs(pattern.getState("q3"))));

        pattern.addTransition(pattern.getState("q0"), pattern.getState("q1"), new CharacterMatcher(uchar('a')));
        pattern.addTransition(pattern.getState("q1"), pattern.getState("q2"), new CharacterMatcher(uchar('b')));
        pattern.addTransition(pattern.getState("q2"), pattern.getState("q2"), new CharacterMatcher(uchar('b')));
        pattern.addTransition(pattern.getState("q2"), pattern.getState("q3"), new EpsilonMatcher());

        std::cout << pattern.matches("abbbbbb") << std::endl;
        std::cout << pattern.matches("aabbbbbb") << std::endl;
        std::cout << pattern.matches("ab") << std::endl;
        std::cout << pattern.matches("a") << std::endl;
    }
    std::cout << "epsilon loop test" << std::endl;
    {
        Pattern pattern;
        pattern.declareStates((make_varargs(string("q0")), "q1", "q2"));
        pattern.setInitialState(pattern.getState("q0"));
        pattern.setFinalStates((make_varargs(pattern.getState("q2"))));

        pattern.addTransition(pattern.getState("q0"), pattern.getState("q1"), new CharacterMatcher(uchar('a')));
        pattern.addTransition(pattern.getState("q1"), pattern.getState("q1"), new EpsilonMatcher());
        pattern.addTransition(pattern.getState("q1"), pattern.getState("q2"), new CharacterMatcher(uchar('b')));

        std::cout << pattern.matches("ab") << std::endl;
    }
    std::cout << "NFA pattern concatenation test" << std::endl;
    {
        Regex r("abc");

        std::cout << r.matches("d") << std::endl;
        std::cout << r.matches("abc") << std::endl;
        std::cout << r.matches(" a b c") << std::endl;
    }
    std::cout << "Full regular expression test" << std::endl;
    {
        Regex r1("(a|b)+c");
        std::cout << r1.matches("abababababacccc") << std::endl;

        Regex r2("(a|b)");
        std::cout << r2.matches("a") << std::endl;
        std::cout << r2.matches("b") << std::endl;
        std::cout << r2.matches("d") << std::endl;
    }
    std::cout << "DONE..." << std::endl;
    std::getchar();
    return (EXIT_SUCCESS);
}

