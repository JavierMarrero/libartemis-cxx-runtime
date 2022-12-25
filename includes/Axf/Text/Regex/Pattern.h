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
 * File:   RegexAutomata.h
 * Author: Javier Marrero
 *
 * Created on December 23, 2022, 1:59 PM
 */

#ifndef REGEXAUTOMATA_H
#define REGEXAUTOMATA_H

// API
#include <Axf/Collections/HashMap.h>
#include <Axf/Core/Lang-C++/varargs.h>
#include <Axf/Core/Object.h>
#include <Axf/Text/Regex/Matcher.h>
#include <Axf/Text/Regex/State.h>

namespace axf
{
namespace text
{
namespace regex
{

/**
 * The <code>Pattern</code> class is the nondeterministic finite state
 * automata used to recognize the regular expressions. Nondeterministic finite
 * state machines are slower but also use less memory, which makes them suitable
 * as regular expression automata for general purposes.
 * <p>
 * This particular implementation uses a simple backtracking implementation.
 *
 * @author J. Marrero
 */
class Pattern : public core::Object
{
    AXF_CLASS_TYPE(axf::text::regex::Pattern,
                   AXF_TYPE(axf::core::Object))
public:

    Pattern();
    ~Pattern();

    /**
     * Adds a new state labeled 'name' to the set of states that this finite
     * state automata has.
     *
     * @param name
     */
    void addState(const core::string& name);

    /**
     * Adds a transition to the nondeterministic finite state automata.
     *
     * @param from
     * @param to
     * @param matcher
     */
    void addTransition(State* from, State* to, Matcher* matcher);

    /**
     * Declares a set of states with the provided 'names' as labels.
     *
     * @param names
     */
    void declareStates(const core::varargs<core::string> names);

    /**
     * Retrieves an state from the set of states of this finite automata.
     *
     * @param name
     * @return
     */
    State* getState(const core::string& name) const;

    /**
     * Checks if the given string matches the regular expression represented
     * by this finite state automata.
     *
     * @param pattern
     * @return
     */
    bool matches(const core::string& pattern) const;

    /**
     * Sets the initial state.
     *
     * @param state
     */
    void setInitialState(State* state);

    /**
     * Sets the final state.
     *
     * @param state
     */
    void setFinalStates(const core::varargs<State*> states);

    /**
     * Assigns the highest priority to a newly added transition.
     *
     * @param from
     * @param to
     * @param number
     */
    void unshiftTransition(State* from, State* to, Matcher* matcher);

private:

    struct stringkey_hasher
    {

        inline unsigned int operator()(const core::string& rhs) const
        {
            return rhs.hash();
        }
    } ;

    typedef collections::HashMap<core::string, State*, stringkey_hasher> StatesMap;
    typedef collections::LinkedList<State*> StatesList;

    StatesList              m_finalStates;      /// Final states
    State*                  m_initialState;     /// Begin state

    StatesMap m_states;                         /// Hash map containing the states

} ;

}
}
}

#endif /* REGEXAUTOMATA_H */

