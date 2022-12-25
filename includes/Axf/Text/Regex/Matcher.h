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
 * File:   Matcher.h
 * Author: Javier Marrero
 *
 * Created on December 23, 2022, 1:17 PM
 */

#ifndef MATCHER_H
#define MATCHER_H

// API
#include <Axf/Core/Object.h>
#include <Axf/Core/String.h>

namespace axf
{
namespace text
{
namespace regex
{

/**
 * The matcher interface defines a medium to "match" some input to some expected
 * state.
 * <p>
 * There are several types of matchers, all complying with this interface, which
 * defines the requirements for matching objects and use custom matchers to
 * extend this engine's semantics.
 *
 * @author J. Marrero
 */
class Matcher : public core::Object
{

    AXF_CLASS_TYPE(axf::text::regex::Matcher,
                   AXF_TYPE(axf::core::Object))
public:

    /**
     * Default constructor for the matcher interface
     *
     * @param label
     */
    Matcher(const core::string& label = "unspecified matcher") : m_label(label) { }

    ~Matcher() { }

    /**
     * Returns the label that identifies this matcher.
     *
     * @return
     */
    inline const core::string& getLabel() const
    {
        return m_label;
    }

    /**
     * Returns true if this is an epsilon transition.
     *
     * @return
     */
    virtual bool isEpsilon() const = 0;

    /**
     * This is a boolean method that returns true if the passed input matches.
     *
     * @param rhs
     * @return
     */
    virtual bool matches(const void* input) const = 0;

private:

    core::string m_label;
} ;

}
}
}

#endif /* MATCHER_H */

