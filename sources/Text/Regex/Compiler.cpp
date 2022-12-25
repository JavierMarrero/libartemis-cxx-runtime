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
 * File:   Compiler.cpp
 * Author: Javier Marrero
 * 
 * Created on December 23, 2022, 9:40 PM
 */

#include <Axf/Text/Regex/Compiler.h>
#include <Axf/Text/Regex/PatternSyntaxException.h>

// C
#include <cstdio>

using namespace axf;
using namespace axf::text;
using namespace axf::text::regex;

Compiler::Compiler(const core::string& input)
:
m_generated(0),
m_input(input),
m_symbol(nullptr)
{
}

Compiler::~Compiler()
{
    for (collections::iterator<Token*> it = m_tokens.begin(),
         end = m_tokens.end(); it != end; it->next())
    {
        delete *it;
    }
}

bool Compiler::accept(RegexToken symbol)
{
    if (m_symbol == nullptr || m_symbol->m_token != symbol)
    {
        return false;
    }

    // Read the next input token
    popToken();
    return true;
}

void Compiler::buildAbstractSyntaxTree()
{
    // 1. Get the next token
    popToken();

    // 2. Annalize it
}

bool Compiler::expect(RegexToken symbol)
{
    if (!accept(symbol))
        raiseSyntaxError(__func__, "unexpected input symbol.");

    return true;
}

core::string Compiler::generateStateName()
{
    char buffer[32] = {0};
    std::sprintf(buffer, "q%d", m_generated++);

    return core::string(buffer);
}

core::strong_ref<Pattern> Compiler::compile()
{
    // 1. Tokenize input
    tokenizeInput();

    // 2. Parse and build an abstract syntax tree
    buildAbstractSyntaxTree();

    // 3. Create the regular automata object.
    core::strong_ref<Pattern> nfa = new Pattern();

    // 4. Annalize the syntax tree and build the nfa

    return nfa;
}

void Compiler::popToken()
{
    if (m_tokens.isEmpty())
        throw PatternSyntaxException("unable to pop token from empty token list.");

    if (m_symbol)
        delete m_symbol;
    m_symbol = m_tokens.poll();
}

void Compiler::raiseSyntaxError(const char* function, const char* message)
{
    char buffer[1024] = {0};
    std::sprintf(buffer, "%s: %s", function, message);

    throw PatternSyntaxException(buffer);
}

/* ==========================================================================
 *                              LEXICAL SCANNER
 * ========================================================================== */

void Compiler::tokenizeInput()
{
    int i = 0;
    core::uchar c = m_input.at(i++);

    while (c != '\0')
    {
        switch (c.asUnicodeEscape())
        {
            case '.':
                pushToken(DOT);
                break;
            case '|':
                pushToken(VBAR);
                break;
            case '(':
                pushToken(LPAR);
                break;
            case ')':
                pushToken(RPAR);
                break;
        }

        // Read a new character from the stream
        c = m_input.at(i++);
    }
}

/* ==========================================================================
 *                          RECURSIVE DESCENT PARSER
 * ========================================================================== */

void Compiler::parseExpression()
{

}

