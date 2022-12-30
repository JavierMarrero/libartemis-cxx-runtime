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
 * File:   AbstractSyntaxTree.cpp
 * Author: Javier Marrero
 * 
 * Created on December 24, 2022, 7:08 PM
 */

#include <Axf/Text/Regex/AbstractSyntaxTree.h>

using namespace axf;
using namespace axf::core;
using namespace axf::text;
using namespace axf::text::regex;
using namespace axf::text::regex::ast;

core::string AstNode::pad(int level)
{
    core::string result;
    result.reserve(level * 2);

    while (level-- > 0)
    {
        result.append("  ");
    }
    return result;
}

AstNode::AstNode(Type type)
:
m_type(type)
{
}

AstNode::~AstNode()
{
}

Atom::Atom()
:
AstNode(ATOM)
{
}

Atom::~Atom()
{
}

Expression::Expression()
:
AstNode(AstNode::EXPRESSION)
{
}

Expression::~Expression()
{
}

Factor::Factor()
:
AstNode(AstNode::FACTOR),
m_metacharacter(UNDEFINED_META)
{
}

Factor::~Factor()
{
}

Term::Term()
:
AstNode(AstNode::TERM)
{
}

Term::~Term()
{
}

Character::Character(const core::uchar& c)
:
AstNode(CHARACTER),
m_character(c)
{
}

Character::~Character()
{
}

void Character::writeString(int ident, core::string& buffer) const
{
    buffer.append(pad(ident)).append("<char>\n");
}

void Atom::writeString(int ident, core::string& buffer) const
{
    buffer.append(pad(ident)).append("<atom>\n");

    m_internal->writeString(++ident, buffer);
}

void Factor::writeString(int ident, core::string& buffer) const
{
    buffer.append(pad(ident)).append("<factor>\n");

    m_atom->writeString(ident + 1, buffer);
    if (hasMetacharacter())
    {
        buffer.append(pad(ident + 1)).append("<'meta'>\n");
    }
}

void Term::writeString(int ident, core::string& buffer) const
{
    buffer.append(pad(ident)).append("<term>\n");
    m_factor->writeString(ident + 1, buffer);

    if (hasNext())
    {
        m_term->writeString(ident, buffer);
    }
}

void Expression::writeString(int ident, core::string& buffer) const
{
    buffer.append(pad(ident)).append("<expression>\n");
    m_lhs->writeString(ident + 1, buffer);

    if (hasAlternative())
    {
        buffer.append(pad(ident + 1)).append("| ");
        m_rhs->writeString(0, buffer);
    }
}

