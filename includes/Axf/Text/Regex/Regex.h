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
 * File:   Regex.h
 * Author: Javier Marrero
 *
 * Created on December 23, 2022, 10:51 PM
 */

#ifndef REGEX_H
#define REGEX_H

// API
#include <Axf/Core/Object.h>
#include <Axf/Text/Regex/Compiler.h>
#include <Axf/Text/Regex/Pattern.h>

namespace axf
{
namespace text
{
namespace regex
{

/**
 * A regular expression defined by a pattern.
 *
 * @author J. Marrero
 */
class Regex : public core::Object
{
    AXF_CLASS_TYPE(axf::text::regex::Regex,
                   AXF_TYPE(axf::core::Object))
public:

    Regex(const core::string& pattern); /// Construct a new regular expression
    ~Regex();                           /// Destructs a regular expression

    /**
     * Returns true if the given regular expression matches the provided input.
     *
     * @param input
     * @return
     */
    bool matches(const core::string& input) const;

private:

    core::strong_ref<Pattern>   m_nfa;      /// The nondeterministic finite state automata
    core::string                m_pattern;  /// The pattern itself as a raw string
} ;

}
}
}

#endif /* REGEX_H */

