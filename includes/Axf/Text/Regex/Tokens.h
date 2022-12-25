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
 * File:   Tokens.h
 * Author: Javier Marrero
 *
 * Created on December 23, 2022, 9:42 PM
 */

#ifndef REGEX_TOKENS_H
#define REGEX_TOKENS_H

namespace axf
{
namespace text
{
namespace regex
{

/**
 * A enumeration of all the possible token categories. Please, keep this list
 * sorted alphabetically.
 *
 * @author J. Marrero
 */
enum RegexToken
{
    ASTERISK,
    DOT,
    EXCLAMATION_MARK,
    INTERROGATION_MARK,
    LPAR,
    PLUS,
    RPAR,
    UNICODE_CHARACTER,
    VBAR,
} ;

}
}
}

#endif /* TOKENS_H */

