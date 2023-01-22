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
 * File:   Compiler.h
 * Author: Javier Marrero
 *
 * Created on December 23, 2022, 9:40 PM
 */

#ifndef REGEX_COMPILER_H
#define REGEX_COMPILER_H

// API
#include <Axf/Collections/LinkedList.h>
#include <Axf/Text/Regex/AbstractSyntaxTree.h>
#include <Axf/Text/Regex/Pattern.h>
#include <Axf/Text/Regex/Tokens.h>

namespace axf
{
namespace text
{
namespace regex
{

/**
 * The compiler class handles the creation of suitable nondeterministic
 * finite automata (<code>Pattern</code> class instances).
 * <p>
 *
 * @author J. Marrero
 */
class Compiler : public core::Object
{

    AXF_CLASS_TYPE(axf::text::regex::Compiler,
                   AXF_TYPE(axf::core::Object))
private:

    /**
     * Represents a token within the compiler. All the tokens are sequentially
     * put in a queue for its consumption by the compiler parser.
     */
    struct Token
    {
        RegexToken      m_token;
        core::uchar     m_value;

        // -- Constructors and destructor --
        // ----------------------------------

        Token(RegexToken type, const core::uchar& value)
        : m_token(type), m_value(value) { }

        Token(RegexToken type) : m_token(type), m_value('\0') { }

        ~Token() { }

        // -- Operator overload and unrelated stuff --
        // -------------------------------------------

        inline bool operator==(const Token& rhs) const
        {
            return m_token == rhs.m_token && m_value == rhs.m_value;
        }

        inline bool operator!=(const Token& rhs) const
        {
            return (*this == rhs) == false;
        }
    } ;

    typedef collections::LinkedList<Token*> TokenQueue;

public:

    Compiler(const core::string& input);    /// Creates a new regular expression compiler
    ~Compiler();                            /// Destroys an instance of a regular expression compiler

    core::strong_ref<Pattern> compile();    /// Compiles the input pattern and produces a NFA suitable for recognizing the regular expression

private:

    static core::strong_ref<Matcher> EPSILON_MATCHER;

    core::strong_ref<ast::AstNode>  m_ast;          /// Abstract syntax tree
    mutable int                     m_generated;    /// Generation Id for states
    const core::string&             m_input;        /// The input string (the regular expression)
    Token*                          m_symbol;       /// The current symbol
    TokenQueue                      m_tokens;       /// The queue of tokens

    /**
     * Accepts a symbol.
     *
     * @param symbol
     */
    bool accept(RegexToken symbol);

    /**
     * Expects the occurrence of a given symbol in the input.
     *
     * @param symbol
     * @return
     */
    bool expect(RegexToken symbol);

    /**
     * Generates a new state name.
     *
     * @return
     */
    core::string generateStateName() const;

    /**
     * Returns if the input has more or less tokens.
     *
     * @return
     */
    bool hasMoreTokens() const;

    /**
     * Peeks the current symbol for exhamination.
     *
     * @return
     */
    inline const Token* peek() const
    {
        return m_symbol;
    }

    /**
     * Reads the next input token.
     */
    Token* popToken();

    /**
     * Pushes a token on the token queue.
     */
    inline bool pushToken(RegexToken type, const core::uchar& value)
    {
        return m_tokens.offer(new Token(type, value));
    }

    /**
     * Pushes a token on the token queue with no semantic value.
     *
     * @param type
     * @return
     */
    inline bool pushToken(RegexToken type)
    {
        return m_tokens.offer(new Token(type));
    }

    /**
     * Raises a <code>PatternSyntaxException</code>.
     *
     * @param function
     * @param message
     */
    void raiseSyntaxError(const char* function, const char* message);

    /**
     * Splits the input strings into processable tokens and fills the queue of
     * tokens with the processed tokens.
     */
    void tokenizeInput();

    /** =========================== PARSING METHODS ===========================
     *
     * WARNING: The following methods are part of the LL(1) recursive descent
     * parser that handles the grammar of the regular expressions. Please,
     * be kind and do not put anything else (no declarations, no shit of any
     * kind) under this code that is not a function that parses a non-terminal
     * symbol on the grammar.
     * 
     * ===================================================================== **/

    ast::AstNode*   parseExpression();
    ast::AstNode*   parseTerm();
    ast::AstNode*   parseFactor();
    ast::AstNode*   parseAtom();

    /** ============================ NFA BUILDER ==============================
     *
     * INFO: The following methods are part of the infrastructure that builds
     * Nondeterministic Finite State Automata from regular expression parse
     * trees.
     *
     * ===================================================================== **/

    core::strong_ref<Pattern> concatenateNfa(Pattern* lhs, Pattern* rhs) const;
    core::strong_ref<Pattern> kleeneClosure(Pattern* arg) const;
    core::strong_ref<Pattern> unionizeNfa(Pattern* lhs, Pattern* rhs) const;

    core::strong_ref<Pattern> generateNfaOneStep(Matcher* matcher) const;
    core::strong_ref<Pattern> generateNfaForExpression(ast::Expression* expression) const;
    core::strong_ref<Pattern> generateNfaForTerm(ast::Term* term) const;
    core::strong_ref<Pattern> generateNfaForFactor(ast::Factor* factor) const;
    core::strong_ref<Pattern> generateNfaForAtom(ast::Atom* atom) const;
} ;

}
}
}

#endif /* COMPILER_H */
