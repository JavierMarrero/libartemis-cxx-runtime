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
#include <Axf/Text/Regex/CharacterMatcher.h>
#include <Axf/Text/Regex/EpsilonMatcher.h>
#include <Axf/Text/Regex/PatternSyntaxException.h>

// C
#include <cstdio>

// C++
#include <iostream>

using namespace axf;
using namespace axf::text;
using namespace axf::text::regex;

core::strong_ref<Matcher> Compiler::EPSILON_MATCHER(new EpsilonMatcher());

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
    if (m_symbol == nullptr || peek()->m_token != symbol)
    {
        return false;
    }

    // Read the next input token
    popToken();
    return true;
}

core::strong_ref<Pattern> Compiler::compile()
{
    // 1. Tokenize input
    tokenizeInput();

    // 2. Parse and build an abstract syntax tree
    m_ast = parseExpression();

//    std::cout << m_ast->toString().bytes() << std::endl;

    // 4. Annalize the syntax tree and build the nfa
    return generateNfaForExpression(static_cast<ast::Expression*> (m_ast.get()));
}

bool Compiler::expect(RegexToken symbol)
{
    if (!accept(symbol))
        raiseSyntaxError(__func__, "unexpected input symbol.");

    return true;
}

core::string Compiler::generateStateName() const
{
    char buffer[32] = {0};
    std::sprintf(buffer, "q%d", m_generated++);

    return core::string(buffer);
}

bool Compiler::hasMoreTokens() const
{
    return m_tokens.isEmpty() == false;
}

Compiler::Token* Compiler::popToken()
{
    if (m_tokens.isEmpty())
        throw PatternSyntaxException("unable to pop token from empty token list.");

    if (m_symbol)
        delete m_symbol;
    m_symbol = m_tokens.poll();

    return m_symbol;
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
    std::size_t i = 0;
    core::uchar c = m_input.at(i++);

    while (c != '\0')
    {
        const unsigned int uc = c.asUnicodeEscape();
        switch (uc)
        {
            case '.':
                pushToken(DOT);
                break;
            case '|':
                pushToken(ALTERNATIVE_BAR);
                break;
            case '+':
                pushToken(PLUS);
                break;
            case '(':
                pushToken(LEFT_PARENTHESIS);
                break;
            case ')':
                pushToken(RIGHT_PARENTHESIS);
                break;
            default:
            {
                pushToken(CHARACTER, c);
            }
        }

        // Read a new character from the stream
        c = m_input.at(i++);
    }

    // Set the first token
    popToken();
}

/* ==========================================================================
 *                          RECURSIVE DESCENT PARSER
 * ========================================================================== */

ast::AstNode* Compiler::parseExpression()
{
    ast::Expression* expr = new ast::Expression();
    ast::AstNode* term = parseTerm();

    if (hasMoreTokens() && peek()->m_token == ALTERNATIVE_BAR)
    {
        accept(ALTERNATIVE_BAR);
        expr->setRhsExpression(parseExpression());
    }

    expr->setTerm(static_cast<ast::Term*> (term));
    return expr;
}

ast::AstNode* Compiler::parseTerm()
{
    ast::Term* term = new ast::Term();
    ast::Factor* factor = static_cast<ast::Factor*> (parseFactor());

    while (hasMoreTokens() && peek()->m_token != RIGHT_PARENTHESIS && peek()->m_token != ALTERNATIVE_BAR)
    {
        term->addTerm(static_cast<ast::Term*> (parseTerm()));
    }

    term->setFactor(factor);
    return term;
}

ast::AstNode* Compiler::parseFactor()
{
    ast::Factor* factor = new ast::Factor();
    ast::Atom* atom = static_cast<ast::Atom*> (parseAtom());

    // Parse metacharacters
    if (hasMoreTokens() && peek()->m_token != RIGHT_PARENTHESIS && peek()->m_token != ALTERNATIVE_BAR)
    {
        bool accept = true;
        switch (peek()->m_token)
        {
            case PLUS:
                factor->setMetacharacter(PLUS);
                break;
            default:
                accept = false;
                break;
        }

        if (accept)
            this->accept(peek()->m_token);
    }

    // Returns
    factor->setAtom(atom);
    return factor;
}

ast::AstNode* Compiler::parseAtom()
{
    ast::Atom* atom = new ast::Atom();

    switch (peek()->m_token)
    {
        case LEFT_PARENTHESIS:
        {
            accept(LEFT_PARENTHESIS);
            atom->set(parseExpression());
            expect(RIGHT_PARENTHESIS);
        }
            break;
        case CHARACTER:
        {
            atom->set(new ast::Character(peek()->m_value));
            accept(CHARACTER);
        }
            break;
        default:
            raiseSyntaxError(__func__, "syntax error on regular expression.");
    }
    return atom;
}
/* ==========================================================================
 *                  NONDETERMINISTIC FINITE AUTOMATA GENERATOR
 * ========================================================================== */

using namespace axf::core;

using ast::AstNode;

core::strong_ref<Pattern> Compiler::concatenateNfa(Pattern* lhs, Pattern* rhs) const
{
    core::strong_ref<State> unionState = lhs->getFinalStates().begin()->current();
    return &lhs->appendPattern(*rhs, unionState);
}

core::strong_ref<Pattern> Compiler::kleeneClosure(Pattern* arg) const
{
    core::strong_ref<Pattern> nfa = new Pattern();
    nfa->makeSuperset(*arg);

    // 1. Generate the initial and final states
    core::string q = generateStateName();
    core::string f = generateStateName();

    nfa->addState(q);
    nfa->addState(f);

    State* qState = nfa->getState(q);
    State* fState = nfa->getState(f);

    // 2. Generate the transitions
    State* nsInitial = arg->getInitialState();
    State* nsFinal = arg->getThompsonConstructFinalState();

    nfa->addTransition(nsFinal, nsInitial, EPSILON_MATCHER.get());
    nfa->addTransition(qState, nsInitial, EPSILON_MATCHER.get());
    nfa->addTransition(qState, fState, EPSILON_MATCHER.get());
    nfa->addTransition(nsFinal, fState, EPSILON_MATCHER.get());

    return nfa;
}

core::strong_ref<Pattern> Compiler::unionizeNfa(Pattern* lhs, Pattern* rhs) const
{
    core::strong_ref<Pattern> nfa = new Pattern();

    // 1. Generate the initial and final states
    core::string q = generateStateName();
    core::string f = generateStateName();

    nfa->addState(q);
    nfa->addState(f);

    State* qState = nfa->getState(q);
    State* fState = nfa->getState(f);

    // 2. The generated automata is a superset of the two arguments
    nfa->makeSuperset(*lhs);
    nfa->makeSuperset(*rhs);

    // 3. Generate the two initial transitions
    State* nsInitial = lhs->getInitialState();
    State* ntInitial = rhs->getInitialState();

    nfa->addTransition(qState, nsInitial, EPSILON_MATCHER.get());
    nfa->addTransition(qState, ntInitial, EPSILON_MATCHER.get());

    // 4. Generate the two final transitions
    State* nsFinal = lhs->getThompsonConstructFinalState();
    State* ntFinal = rhs->getThompsonConstructFinalState();

    nfa->addTransition(nsFinal, fState, EPSILON_MATCHER.get());
    nfa->addTransition(ntFinal, fState, EPSILON_MATCHER.get());

    // 5. Set the correct initial and final states
    nfa->setInitialState(qState);
    nfa->setFinalStates(make_varargs(fState));

    return nfa;
}

core::strong_ref<Pattern> Compiler::generateNfaOneStep(Matcher* matcher) const
{
    strong_ref<Pattern> nfa = new Pattern();

    string a = generateStateName();
    string b = generateStateName();

    nfa->declareStates((make_varargs(a), b));
    nfa->setInitialState(a);
    nfa->setFinalStates(make_varargs(nfa->getState(b)));

    nfa->addTransition(nfa->getState(a), nfa->getState(b), matcher);
    return nfa;
}

core::strong_ref<Pattern> Compiler::generateNfaForExpression(ast::Expression* expression) const
{
    core::strong_ref<Pattern> lhs = generateNfaForTerm(expression->getLeftHandSideExpression());
    if (expression->hasAlternative())
    {
        return unionizeNfa(lhs.get(), generateNfaForExpression(expression->getRightHandSideExpression()).get());
    }
    else
    {
        return lhs;
    }
}

core::strong_ref<Pattern> Compiler::generateNfaForTerm(ast::Term* term) const
{
    core::strong_ref<Pattern> nfa = generateNfaForFactor(term->getFactor());

    if (term->hasNext())
    {
        concatenateNfa(nfa.get(), generateNfaForTerm(term->getNextTerm()).get());
    }
    return nfa;
}

core::strong_ref<Pattern> Compiler::generateNfaForFactor(ast::Factor* factor) const
{
    // 1. Generate the NFA for the atom
    strong_ref<Pattern> atom = generateNfaForAtom(static_cast<ast::Atom*> (factor->getAtom()));

    // 2. If has a metacharacter generate the automata
    if (factor->hasMetacharacter())
    {
        switch (factor->getMetacharacter())
        {
            case PLUS:
            {


                break;
            }
        }
    }

    return atom;
}

core::strong_ref<Pattern> Compiler::generateNfaForAtom(ast::Atom* atom) const
{
    core::strong_ref<Pattern> pattern;

    switch (atom->peekType())
    {
        case (AstNode::CHARACTER):
        {
            ast::Character* c = static_cast<ast::Character*> (atom->get());
            pattern = generateNfaOneStep(new CharacterMatcher(c->getCharacter()));

            break;
        }
        case (AstNode::EXPRESSION):
        {
            pattern = generateNfaForExpression(static_cast<ast::Expression*> (atom->get()));
            break;
        }
        default:
            break;
    }

    return pattern;
}


