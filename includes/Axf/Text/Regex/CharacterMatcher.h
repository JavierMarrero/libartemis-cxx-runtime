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
 * File:   CharacterMatcher.h
 * Author: Javier Marrero
 *
 * Created on December 23, 2022, 1:27 PM
 */

#ifndef CHARACTERMATCHER_H
#define CHARACTERMATCHER_H

// API
#include <Axf/Core/Object.h>
#include <Axf/Text/Regex/Matcher.h>

namespace axf
{
namespace text
{
namespace regex
{

/**
 * A matcher that handles Unicode character input.
 */
class CharacterMatcher : public Matcher
{
    AXF_CLASS_TYPE(axf::text::regex::CharacterMatcher,
                   AXF_TYPE(axf::text::regex::Matcher))

public:

    CharacterMatcher(const core::uchar& c);
    ~CharacterMatcher();

    virtual bool isEpsilon() const;
    virtual bool matches(const void* input) const;

private:

    core::uchar m_character;
} ;

}
}
}

#endif /* CHARACTERMATCHER_H */

