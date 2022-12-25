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
 * File:   Regex.cpp
 * Author: Javier Marrero
 * 
 * Created on December 23, 2022, 10:51 PM
 */

#include <Axf/Text/Regex/Regex.h>
#include <Axf/Text/Regex/Compiler.h>
#include <Axf/Core/NullPointerException.h>

using namespace axf;
using namespace axf::text;
using namespace axf::text::regex;

Regex::Regex(const core::string& pattern)
:
m_pattern(pattern)
{
    // 1. Create a compiler object and compile the pattern into a NFA
    Compiler compiler(m_pattern);

    // 2. Create the NFA instance
    m_nfa = compiler.compile();
}

Regex::~Regex()
{
}

bool Regex::matches(const core::string& input) const
{
    if (m_nfa.isNull())
        throw core::NullPointerException("NFA for regex is null.");

    return m_nfa->matches(input);
}

