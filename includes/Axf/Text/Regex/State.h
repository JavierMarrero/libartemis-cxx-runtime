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
 * File:   State.h
 * Author: Javier Marrero
 *
 * Created on December 23, 2022, 1:11 PM
 */

#ifndef STATE_H
#define STATE_H

// API
#include <Axf/Collections/LinkedList.h>
#include <Axf/Core/Object.h>
#include <Axf/Core/String.h>
#include <Axf/Text/Regex/Matcher.h>
#include <Axf/Utils/Pair.h>

namespace axf
{
namespace text
{
namespace regex
{

/**
 * A "state" represents a state in the non deterministic finite state regular
 * automata that is used in regular expression pattern matching. A state can
 * be defined as a set of transitions.
 * <p>
 * 
 *
 * @author J. Marrero
 */
class State : public core::Object
{
    AXF_CLASS_TYPE(axf::text::regex::State,
                   AXF_TYPE(axf::core::Object))
public:

    typedef utils::Pair<core::strong_ref<Matcher>, core::strong_ref<State> > Transition;
    typedef collections::LinkedList<Transition> TransitionList;

    State(const core::string& name);
    ~State();

    /**
     * Adds a new transition to this state. Transitions are simply pushed onto
     * a stack.
     *
     * @param to
     * @param matcher
     */
    void addTransition(State* to, Matcher* matcher);

    /**
     * Returns a reference to the list of transitions.
     *
     * @return
     */
    inline TransitionList& transitions()
    {
        return m_transitions;
    }

    /**
     * Adds a new transition and assigns the maximum priority.
     *
     * @param to
     * @param matcher
     */
    void unshiftTransition(State* to, Matcher* matcher);

private:

    core::string    m_name;         /// The name of the state
    TransitionList  m_transitions;  /// The transitions themselves

} ;

}
}
}

#endif /* STATE_H */

