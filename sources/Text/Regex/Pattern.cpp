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
 * File:   RegexAutomata.cpp
 * Author: Javier Marrero
 * 
 * Created on December 23, 2022, 1:59 PM
 */

#include <Axf/Text/Regex/Pattern.h>
#include <Axf/Collections/HashSet.h>

using namespace axf;
using namespace axf::collections;
using namespace axf::text::regex;

Pattern::Pattern()
:
m_initialState(nullptr)
{
}

Pattern::~Pattern()
{
    if (m_states.isEmpty() == false)
    {
        for (const iterator<Entry<core::string, State*> > it = m_states.begin(),
             end = m_states.end(); it != end; it->next())
        {
            delete it->current().getValue();
        }
    }

    // Other parameters
    m_initialState = nullptr;
}

void Pattern::addState(const core::string& name)
{
    m_states.put(name, new State(name));
}

void Pattern::addTransition(State* from, State* to, Matcher* matcher)
{
    from->addTransition(to, matcher);
}

void Pattern::declareStates(const core::varargs<core::string> names)
{
    for (std::size_t n = 0; n < names.length(); ++n)
    {
        addState(names[n]);
    }
}

State* Pattern::getState(const core::string& name) const
{
    return const_cast<State*> (m_states.get(name));
}

bool Pattern::matches(const core::string& pattern) const
{
    if (m_initialState == nullptr || m_finalStates.isEmpty() || m_states.isEmpty())
        return false;

    collections::LinkedList<utils::Pair<std::size_t, State*> > stack;
    stack.push(utils::makePair((std::size_t) 0ul, const_cast<State*> (m_initialState)));

    // Memory
    HashSet<State*> EPSILON_VISITED;
    // --

    while (stack.isEmpty() == false)
    {
        utils::Pair<std::size_t, State*> lastPair = stack.peek();
        stack.pop();

        std::size_t i = lastPair.first();
        State* currentState = lastPair.second();

        if (m_finalStates.contains(currentState))
        {
            return true;
        }

        core::uchar input = pattern.at(i);

        // Transitions are pushed in reverse order because we want the first transition
        // to be in the last position of the stack. Stacks are LIFO (Last In, First Out)
        // TODO: Change this for a reverse iterator
        for (long long c = (long long) currentState->transitions().size() - 1;
             c >= 0; --c)
        {
            State::Transition& currentTransition = currentState->transitions().get(c);
            Matcher* matcher = currentTransition.first();
            State* toState = currentTransition.second();

            if (matcher->matches(static_cast<void*> (&input)))
            {
                if (matcher->isEpsilon())
                {
                    // Don't follow the transition. We already have been in that state
                    if (EPSILON_VISITED.contains(toState))
                    {
                        continue;
                    }

                    // Remember that you made this transition
                    EPSILON_VISITED.add(currentState);
                }
                else
                {
                    // We are traversing a non-epsilon transition, one may
                    // reset the visited counter
                    EPSILON_VISITED.clear();
                }

                // Reminder: Epsilon transitions don't consume input
                if (matcher->isEpsilon() == false) i++;

                stack.push(utils::makePair(i, toState));
            }
        }
    }
    return false;
}

void Pattern::setFinalStates(const core::varargs<State*> states)
{
    for (std::size_t i = 0; i < states.length(); ++i)
    {
        m_finalStates.add(states[i]);
    }
}

void Pattern::setInitialState(State* state)
{
    this->m_initialState = state;
}

void Pattern::unshiftTransition(State* from, State* to, Matcher* matcher)
{
    from->unshiftTransition(to, matcher);
}


