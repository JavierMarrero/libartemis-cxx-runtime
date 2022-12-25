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
 * File:   CharacterMatcher.cpp
 * Author: Javier Marrero
 * 
 * Created on December 23, 2022, 1:27 PM
 */

#include <Axf/Text/Regex/CharacterMatcher.h>

using namespace axf;
using namespace axf::text;
using namespace axf::text::regex;

CharacterMatcher::CharacterMatcher(const core::uchar& c)
:
Matcher(c),
m_character(c)
{
}

CharacterMatcher::~CharacterMatcher()
{
}

bool CharacterMatcher::isEpsilon() const
{
    return false;
}

bool CharacterMatcher::matches(const void* input) const
{
    const core::uchar& c = *((core::uchar*) input);
    return this->m_character == c;
}




