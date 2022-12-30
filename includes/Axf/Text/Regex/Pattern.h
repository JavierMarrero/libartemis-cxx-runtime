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
private:

    /**
     * This struct performs the mapping between strings and key hashes.
     *
     * @param rhs
     * @return
     */
    struct stringkey_hasher
    {

        inline unsigned int operator()(const core::string& rhs) const
        {
            return rhs.hash();
        }
    } ;

    typedef collections::HashMap<core::string, core::strong_ref<State>, stringkey_hasher> StatesMap;
    typedef collections::LinkedList<State*> StatesList;

public:

    Pattern();
    ~Pattern();

    /**
     * Sets a new state as a final state. It does not add the state so the state
     * must be part of the list of states of the pattern.
     *
     * @param state
     */
    void addFinalState(const State* state);

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
     * Appends two nondeterministic finite automata. This method should be used
     * only when:
     * <ul>
     *  <li>The initial state of the other NFA doesn't have any inwards transitions</li>
     *  <li><code>unionState</code> doesn't have any outwards transition</li>
     * </ul>
     * <p>
     * This method allows to implement Thompson construction. If both assertions
     * are not true, the append is undefined behavior; but no warnings nor errors
     * will be issued whatsoever in order to gain some speed.
     *
     * @param otherNfa
     * @param nfa
     * @return
     */
    Pattern& appendPattern(Pattern& otherNfa, core::strong_ref<State> unionState);

    /**
     * Declares a set of states with the provided 'names' as labels.
     *
     * @param names
     */
    void declareStates(const core::varargs<core::string> names);

    /**
     * Returns a reference to the list of final states.
     *
     * @return
     */
    inline const StatesList& getFinalStates() const
    {
        return m_finalStates;
    }

    /**
     * Returns a pointer to a state that is suitable to be used in a Thompson
     * construct.
     * 
     * @return
     */
    inline State* getThompsonConstructFinalState()
    {
        return m_finalStates.peek();
    }

    /**
     * Gets the initial state of this automata.
     *
     * @return
     */
    inline State* getInitialState()
    {
        return m_initialState;
    }

    /**
     * Gets the initial state of this automata (const version).
     *
     * @return
     */
    inline const State* getInitialState() const
    {
        return m_initialState;
    }

    /**
     * Retrieves an state from the set of states of this finite automata.
     *
     * @param name
     * @return
     */
    State* getState(const core::string& name) const;

    /**
     * Makes all the states of the argument automata belong to this automata.
     * <p>
     * This method provokes that the automata given as argument a strict subset
     * of this object. No initial or final states are modified in the process
     * for <i>this</i> object.
     *
     * @param pattern
     */
    void makeSuperset(Pattern& pattern);

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
     * Sets the initial state of this NFA by name.
     *
     * @param name
     */
    inline void setInitialState(const core::string& name)
    {
        setInitialState(getState(name));
    }

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

    StatesList              m_finalStates;      /// Final states
    State*                  m_initialState;     /// Begin state

    StatesMap m_states;                         /// Hash map containing the states

} ;

}
}
}

#endif /* REGEXAUTOMATA_H */

