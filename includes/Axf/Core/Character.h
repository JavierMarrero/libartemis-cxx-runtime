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
 * File:   Character.h
 * Author: Javier Marrero
 *
 * Created on December 14, 2022, 11:21 PM
 */

#ifndef CHARACTER_H
#define CHARACTER_H

// API
#include <Axf/Core/ReferenceCounted.h>
#include <Axf/Core/IllegalStateException.h>

// C
#include <cstddef>
#include <climits>

namespace axf
{
namespace io
{
namespace charset
{

/* ================================ NOTICE =====================================
 * 
 * The following set of definitions correspond to the supported character
 * encodings, predating the definitions of GNU libiconv, which this library
 * uses.
 * Please, do not change them without notice nor modify the actual meaning of
 * these definitions, or one may break source or binary compatibility by doing
 * so.
 *
 * ========================================================================== */

extern const char* const ASCII_CHARSET;     /// ASCII standard
extern const char* const ISO_8859_1;        /// Latin1 (ISO-8859-1)
extern const char* const UTF8_CHARSET;      /// UTF-8
extern const char* const UTF16_CHARSET;     /// UTF-16 (machine dependent)
extern const char* const UTF32_CHARSET;     /// UTF-32 (machine dependent)
extern const char* const WINDOWS_CP1252;    /// Windows' CP1252

}
}

namespace core
{

/**
 * A character represents an encoded <i>Unicode</i> code point.
 * <p>
 * Character objects may be created from UTF-8, UTF-16 or UTF-32 byte sequences.
 * They represent encoded Unicode code points. They are comparable and
 * assignable units, and one can perform arithmetic operations on them as
 * any other integer.
 * <p>
 * Internally, they are stored as a 32-bit integer (though the Unicode standard
 * allows for using only 21-bit code points). They can be encoded and decoded
 * in many character sets, so this class acts as an homogeneous handle to
 * the concept of character.
 * <p>
 * Internal format of character entries is UCS-4 little-endian. They can be
 * converted between architectures and conversion functions will always return
 * their results in the adequate encoding for the platform one's running in.
 *
 * @author J. Marrero
 */
class uchar : public ReferenceCounted
{
    typedef unsigned int unicode_t;

    /**
     * This union represents an encoded Unicode character in the machine's
     * target endianness. It can be used directly for printing on screen or
     * wherever a 'printable' version of an Unicode character has to be used.
     * <p>
     * This is the principal goal of this structure, use the methods within
     * the <code>Character</code> class if suitable.
     * <p>
     * This class appends a null terminator in order to be able to return the
     * multi byte sequence as a pointer.
     *
     * @author J. Marrero
     */
    typedef struct mb_char
    {
        wchar_t bytes[3];

        mb_char()
        {
            bytes[0] = 0;
            bytes[1] = 0;
            bytes[2] = 0;
        }

        /**
         * General implicit conversion to a wide character string.
         *
         * @return
         */
        inline operator const wchar_t*() const
        {
            return bytes;
        }
    } mb_char_t;

    static const char little_endian = 1;
    static const char big_endian = 0;

public:

    uchar(const void* data, std::size_t length, const char* charset);
    uchar(const uchar& rhs);

    explicit uchar(const char c);
    explicit uchar(const wchar_t wc);

    ~uchar();

    // METHODS
    bool isAscii() const;       /// Returns true if the character is an ASCII char
    bool isWhitespace() const;  /// Returns true if the character is a white-space

    // INLINE METHOD

    /**
     * Returns this character as a Unicode escape code: a four digit number
     * representing the Unicode code point this character represents.
     *
     * @return
     */
    inline unsigned int asUnicodeEscape() const
    {
        return static_cast<unsigned int> (decodeInternal());
    }

    /**
     * Converts this Unicode code point to a sequence of wide characters with
     * correct endianness and encoding. The returned structure is implicitly
     * convertible to a wchar_t pointer.
     *
     * @return
     */
    mb_char_t toWideCharacter() const;

    // COMPARISON OPERATORS

    inline bool operator==(const uchar& rhs) const
    {
        return m_character == rhs.m_character;
    }

    inline bool operator!=(const uchar& rhs) const
    {
        return m_character != rhs.m_character;
    }

    // CONVERSION OPERATORS

    inline operator char() const
    {
        unicode_t converted = (getEndianness() == big_endian) ?
            m_character : swap(m_character);
        if (converted > CHAR_MAX)
        {
            throw IllegalStateException("encoded code point is outside range of ASCII char.");
        }
        return converted;
    }

    // ARITHMETIC OPERATORS

    uchar& operator+(const int rhs)
    {
        m_character += rhs;
        return *this;
    }

    uchar& operator-(const int rhs)
    {
        m_character -= rhs;
        return *this;
    }

private:

    static const char* const INTERNAL_ENCODING;

    unicode_t m_character;

    /**
     * Internally decodes the UCS-4BE character and returns an endianness-correct
     * representation of the Unicode code point. The format will still be UCS-4,
     * just that with the correct endianness.
     *
     * @return
     */
    inline unicode_t decodeInternal() const
    {
        return (getEndianness() == big_endian) ? m_character : swap(m_character);
    }

    unicode_t encode(const void* memory, const char* charsetId, std::size_t length);

    /**
     * Returns the constant that identifies the system endianness.
     * 
     * @return
     */
    static char getEndianness();

    /**
     * Swaps the endianness of an unsigned integer.
     *
     * @param c
     * @return
     */
    static unicode_t swap(unicode_t c);

} ;

}
}

#endif /* CHARACTER_H */

