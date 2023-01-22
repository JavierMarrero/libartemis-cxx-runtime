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
 * File:   Character.cpp
 * Author: Javier Marrero
 * 
 * Created on December 14, 2022, 11:21 PM
 */

#include <Axf/Core/Character.h>
#include <Axf/Core/IllegalArgumentException.h>
#include <Axf/Core/IllegalStateException.h>
#include <Axf/Core/Runtime.h>

// C++
#include <cerrno>

// ICONV
#include <iconv.h>

// Actual definitions for character set names, see character.cpp file
//

using namespace axf::io;

const char* const axf::io::charset::ASCII_CHARSET = "ASCII";
const char* const axf::io::charset::ISO_8859_1 = "ISO-8859-1";
const char* const axf::io::charset::UTF8_CHARSET = "UTF-8";
const char* const axf::io::charset::UTF16_CHARSET = "UTF-16";
const char* const axf::io::charset::UTF32_CHARSET = "UTF-32";
const char* const axf::io::charset::WINDOWS_CP1252 = "CP1252";

//
// -- end --

using namespace axf;
using namespace axf::core;

const char* const axf::core::uchar::INTERNAL_ENCODING = "UCS-4BE";

uchar::uchar(const void* data, std::size_t length, const char* charset)
:
m_character(encode(data, charset, length))
{
}

uchar::uchar(const uchar& rhs)
:
m_character(rhs.m_character)
{
}

uchar::uchar(const char c)
:
m_character(encode(&c, "char", sizeof (char)))
{
}

uchar::uchar(const wchar_t wc)
:
m_character(encode(&wc, "wchar_t", sizeof (wchar_t)))
{
}

uchar::~uchar()
{
}

void uchar::decode(const char* const encoding, void* memory, std::size_t length) const
{
    iconv_t iv = iconv_open(encoding, INTERNAL_ENCODING);
    if (iv == (iconv_t) (-1))
    {
        throw IllegalArgumentException("unsupported source charset '%s' for decoding character type.", encoding);
    }

    char* inbuf = reinterpret_cast<char*> ((unicode_t*) &m_character);
    char* outbuf = static_cast<char*> (memory);

    std::size_t inSize = 4ul;
    std::size_t outSize = length;

    std::size_t result = iconv(iv, &inbuf, &inSize, &outbuf, &outSize);
    iconv_close(iv);

    if (result == (std::size_t)(-1))
    {
        const char* exceptionMessage = "unknown exception on iconv conversion.";
        switch (errno)
        {
            case EILSEQ:
                exceptionMessage = "an invalid multi-byte sequence is encountered in the input.";
                break;
            case EINVAL:
                exceptionMessage = "an incomplete multi-byte sequence is encountered in the input.";
                break;
            case E2BIG:
                exceptionMessage = "the output buffer has no more room for the next converted character.";
                break;
        }
        throw IllegalStateException(exceptionMessage);
    }
}

uchar::unicode_t uchar::encode(const void* memory, const char* charsetId, std::size_t length /* = 1 */)
{
    const char* bytes = static_cast<const char*> (memory);

    iconv_t iv = iconv_open(INTERNAL_ENCODING, charsetId);
    if (iv == (iconv_t) (-1))
    {
        throw IllegalArgumentException("unsupported source charset '%s' for decoding character type.", charsetId);
    }

    char* inbuf = const_cast<char*> (bytes);
    char* outbuf = reinterpret_cast<char*> (&m_character);

    std::size_t inSize = length;
    std::size_t outSize = 4ul;

    std::size_t result = iconv(iv, &inbuf, &inSize, &outbuf, &outSize);
    iconv_close(iv);

    if (result == (std::size_t)(-1))
    {
        const char* exceptionMessage = "unknown exception on iconv conversion.";
        switch (errno)
        {
            case EILSEQ:
                exceptionMessage = "an invalid multi-byte sequence is encountered in the input.";
                break;
            case EINVAL:
                exceptionMessage = "an incomplete multi-byte sequence is encountered in the input.";
                break;
            case E2BIG:
                exceptionMessage = "the output buffer has no more room for the next converted character.";
                break;
        }
        throw IllegalStateException(exceptionMessage);
    }

    return m_character;
}

char uchar::getEndianness()
{
    return (Runtime::getSystemEndianness() == Runtime::BIG_ENDIAN) ? big_endian : little_endian;
}

bool uchar::isAscii() const
{
    return decodeInternal() > 0x7F;
}

bool uchar::isWhitespace() const
{
    switch (decodeInternal())
    {
        case 0x09:
        case 0x0b:
        case 0x0c:
        case 0x0d:
            // space would be here but we have reserved it for the last true value
        case 0xa0:
        case 0x1680:
        case 0x2000:
        case 0x2001:
        case 0x2002:
        case 0x2003:
        case 0x2004:
        case 0x2005:
        case 0x2006:
        case 0x2007:
        case 0x2008:
        case 0x2009:
        case 0x200a:
        case 0x2028:
        case 0x2029:
        case 0x202f:
        case 0x205f:
        case 0x3000:
        case 0xfeff:
        case 0x20:
            return true;
        default:
            return false;
    }
}

uchar::unicode_t uchar::swap(unicode_t c)
{
    return ((c >> 24) & 0xff)
            | ((c << 8) & 0xff0000)
            | ((c >> 8) & 0xff00)
            | ((c << 24) & 0xff000000);
}

uchar::mb_char_t uchar::toWideCharacter() const
{
    mb_char_t result;

    iconv_t iv = iconv_open("wchar_t", INTERNAL_ENCODING);

    char* inbuf = const_cast<char*> (reinterpret_cast<const char*> (&m_character));
    char* outbuf = reinterpret_cast<char*> (&result.bytes);

    std::size_t inSize = 4ul;
    std::size_t outSize = 2 * sizeof (wchar_t);

    std::size_t conversionResult = iconv(iv, &inbuf, &inSize, &outbuf, &outSize);
    iconv_close(iv);

    if (conversionResult == (std::size_t)(-1))
    {
        const char* exceptionMessage = "unknown exception on iconv conversion.";
        switch (errno)
        {
            case EILSEQ:
                exceptionMessage = "an invalid multi-byte sequence is encountered in the input.";
                break;
            case EINVAL:
                exceptionMessage = "an incomplete multi-byte sequence is encountered in the input.";
                break;
            case E2BIG:
                exceptionMessage = "the output buffer has no more room for the next converted character.";
                break;
        }
        throw IllegalStateException(exceptionMessage);
    }

    return result;
}
