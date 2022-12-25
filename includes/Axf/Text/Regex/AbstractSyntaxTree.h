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
 * File:   AbstractSyntaxTree.h
 * Author: Javier Marrero
 *
 * Created on December 24, 2022, 7:08 PM
 */

#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

// API
#include <Axf/Core/Object.h>

namespace axf
{
namespace text
{
namespace regex
{
namespace ast
{

/**
 * Abstract syntax tree node general base class.
 * <p>
 * This class acts as a base class for the annotated parse tree generated by
 * the regular expression pattern compiler. This tree will be then parsed and
 * a nondeterministic finite state automata representing the described regular
 * expression will be created.
 * <p>
 * This class is part of the regular expression package private API and it is
 * not destined to be used by the general public. As such, the rest of the
 * classes within this namespace will probably be undocumented.
 *
 * @author J. Marrero
 */
class AstNode
{
public:

    /**
     * Enumerates all the possible classes of Abstract Syntax Tree nodes.
     */
    enum Type
    {
        ATOMIC_PATTERN,
        QUANTIFIER,
        REGEX,
        REGEX_ALTERNATIVE
    } ;

    AstNode(Type type);
    ~AstNode();

    /**
     * Returns the type of this node. The type is set up at creation time by
     * the deriving classes.
     *
     * @return
     */
    inline Type getType() const
    {
        return m_type;
    }

protected:

    Type    m_type;
} ;

class AtomicPattern : public AstNode
{
public:

    AtomicPattern(const core::uchar& c);

    /**
     * Returns the "atom" of this node.
     *
     * @return
     */
    inline core::uchar& getAtom() const
    {
        return m_character;
    }

private:

    core::uchar m_character;
} ;

}
}
}
}

#endif /* ABSTRACTSYNTAXTREE_H */
