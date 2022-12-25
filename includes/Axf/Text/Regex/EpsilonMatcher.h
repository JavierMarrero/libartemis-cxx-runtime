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
 * File:   EpsilonMatcher.h
 * Author: Javier Marrero
 *
 * Created on December 23, 2022, 1:42 PM
 */

#ifndef EPSILONMATCHER_H
#define EPSILONMATCHER_H

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
 * Empty matcher.
 */
class EpsilonMatcher : public Matcher
{
    AXF_CLASS_TYPE(axf::text::regex::EpsilonMatcher,
                   AXF_TYPE(axf::text::regex::Matcher))
public:

    EpsilonMatcher();
    ~EpsilonMatcher();

    virtual bool isEpsilon() const;
    virtual bool matches(const void* input) const;

} ;

}
}
}

#endif /* EPSILONMATCHER_H */

