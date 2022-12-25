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
 * File:   State.cpp
 * Author: Javier Marrero
 * 
 * Created on December 23, 2022, 1:11 PM
 */

#include <Axf/Text/Regex/State.h>

using namespace axf;
using namespace axf::text;
using namespace axf::text::regex;

State::State(const core::string& name)
:
m_name(name)
{
}

State::~State()
{
    for (collections::iterator<Transition> it = m_transitions.begin(),
         end = m_transitions.end(); it != end; it->next())
    {
        delete it->current().first();
    }
}

void State::addTransition(State* to, Matcher* matcher)
{
    m_transitions.add(utils::makePair(matcher, to));
}

void State::unshiftTransition(State* to, Matcher* matcher)
{
    m_transitions.add(0, utils::makePair(matcher, to));
}


