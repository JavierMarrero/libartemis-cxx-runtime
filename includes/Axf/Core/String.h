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
 * File:   String.h
 * Author: Javier Marrero
 *
 * Created on November 27, 2022, 6:11 PM
 */

#ifndef AXF_STRING_H
#define AXF_STRING_H

// C++
#include <cstddef>
#include <cstring>

// API
#include <Axf/API/Platform.h>
#include <Axf/Core/Memory.h>
#include <Axf/Core/ReferenceCounted.h>
#include <Axf/Core/Character.h>

namespace axf
{
namespace core
{

// Forward declaration of object class
class Object;

/**
 * Represents the string data type: a finite and null terminated sequence
 * of characters.
 * <p>
 * The <code>string</code> class is akin to that in another programming
 * languages, such as Java and C# (type string). It defines a finite and delimited
 * sequence of characters. Internally, this class uses UTF-8 encoding to support
 * <i>Unicode</i>. UTF-8, being a variable character encoding, provides poor
 * performance for random access (in the order of <code>O(n)</code> being
 * <i>n</i> the number of characters. To mitigate the impact of those operations,
 * this class makes use of some variables, so, they are slightly heavier than
 * plain C character arrays. However, we believe that advantages of easy
 * internationalization, multi-platform support and inter-operability far exceeds
 * the penalties in memory consumption. For text processing utilities, consider
 * using a 'cord-like' data structure, since they provide better complexities
 * for almost every operation.
 * <p>
 * This class comes shipped with several string utilities, namely, substitutes
 * for all the <code>string.h</code> operations (courtesy of the <i>C</i>
 * programming language) and additions to this library such as string replacement
 * and a slightly more advance pattern matching.
 * <p>
 * This class also implements operator overloading for some common operations,
 * making it easier to use than, for example, Java's implementation.
 * <p>
 * This class extends the <code>ReferenceCounted</code> class, so this class
 * is suitable for intrusive reference counting.
 *
 * @author J. Marrero
 */
class string : protected ReferenceCounted
{
public:

    /**
     * An integer value representing an invalid position in the string. It is
     * normally the return value of methods that need to find or return an index
     * and the index returned is not valid somehow.
     */
    static const int NPOS = -1;

    string();                       /// Default constructor
    string(const string& rhs);      /// Default copy constructor
    ~string();                      /// This class' destructor is not marked virtual on purpose

    string(const char* cstr, const char* charset = axf::io::charset::UTF8_CHARSET); /// Constructs a string via a pointer to a c string and a charset
    string(const wchar_t* wstr);                                                    /// Constructs a string via a wide character array

    /**
     * Appends a character to the string.
     * 
     * @param c
     * @return
     */
    string& append(const char c);

    /**
     * Appends 'str' to this string. This is a mutator method and the reference
     * to the string must itself not be constant. The mutated string is the
     * same calling string.
     *
     * @param str
     * @return a reference to "this"
     */
    string& append(const string& str);

    /**
     * Appends 'str' to this string.
     *
     * @param str
     * @return
     */
    string& append(const char* str);

    /**
     * Returns a pointer to a wide character string representing this string.
     *
     * @return
     */
    inline const wchar_t* asWideString() const
    {
        return m_wide.get();
    }

    /**
     * Returns the character at the given index on the string. Characters are
     * encoded from UTF-8 form.
     *
     * @param index
     * @return
     */
    uchar at(std::size_t index) const;

    /**
     * Returns the bytes of this string as a pointer to char.
     * 
     * @return
     */
    inline const char* bytes() const
    {
        return reinterpret_cast<const char*> (m_buffer);
    }

    /**
     * Clear this string's content, releasing all the memory allocated by this
     * object. Memory will be re-allocated if deemed necessary (when mutating
     * this string).
     */
    void clear();

    /**
     * Compares two strings for equality. Two strings are equal if their buffers
     * compare byte-to-byte as equals.
     * <p>
     * If the object argument is a string, then a direct comparison is ensued.
     * If the object is of another kind, then the object is converted to string
     * and then these two get compared.
     *
     * @param rhs
     * @return
     */
    bool equals(const axf::core::Object& rhs) const;

    /**
     * Compares two strings for equality. Two strings are equals if their byte
     * buffers are equals.
     *
     * @param rhs
     * @return
     */
    bool equals(const string& rhs) const;

    /**
     * Hashes this string's contents, a unique (within the restrictions of the
     * use of an imperfect hash functions) integer representing the string is
     * returned.
     *
     * @return
     */
    unsigned int hash() const;

    /**
     * Returns the length of this string in characters, not counting the
     * terminating null.
     * 
     * @return a size_t representing the length of the string (in characters)
     */
    inline size_t length() const
    {
        return m_length;
    }

    /**
     * Ensures that this string has at least n bytes allocated to it. It also
     * allocates enough space to contain a string terminator.
     *
     * @param nbytes
     */
    inline void reserve(std::size_t nbytes)
    {
        ensureCapacity(nbytes + 1);
    }

    /**
     * Returns the length of this string in bytes, not counting the string
     * terminator.
     *
     * @return
     */
    inline size_t size() const
    {
        return m_size;
    }

    /**
     * This class is implicitly usable where a const char pointer is requested.
     * Notice how the buffer is not encoded in the default C locale or any
     * specific encoding, other than UTF-8.
     *
     * @return
     */
    inline operator const char*() const
    {
        return bytes();
    }

    /**
     * Assignment operator overload.
     * 
     * @param rhs
     * @return
     */
    inline string& operator=(const string& rhs)
    {
        if (rhs == *this)
            return *this;

        m_capacity = rhs.m_capacity;
        m_length = rhs.m_length;
        m_size = rhs.m_size;
        m_watermark.m_index = rhs.m_watermark.m_index;
        m_watermark.m_position = rhs.m_watermark.m_position;

        // Allocate a new buffer and copy the contents
        // delete the previous buffer
        delete[] m_buffer;

        m_buffer = new utf8_char[m_capacity];
        std::memcpy(m_buffer, rhs.m_buffer, rhs.m_size);

        return *this;
    }

    /**
     * Compares two strings for equality.
     *
     * @param rhs
     * @return
     */
    inline bool operator==(const string& rhs) const
    {
        return std::strcmp(reinterpret_cast<const char*> (m_buffer), reinterpret_cast<const char*> (rhs.m_buffer)) == 0;
    }

    /**
     * Compares a string and a C string for equality.
     *
     * @param cstr
     * @return
     */
    inline bool operator==(const char* cstr) const
    {
        return std::strcmp(reinterpret_cast<const char*> (m_buffer), cstr) == 0;
    }

private:

    /**
     * A watermark represents the last read or written byte in the string.
     * It allows for a slightly faster indexing, mostly useful on consecutive
     * iteration.
     */
    typedef struct watermark
    {
        std::size_t m_index;
        std::size_t m_position;

        watermark(std::size_t index, std::size_t position) : m_index(index), m_position(position) { }

        watermark(const watermark& rhs) : m_index(rhs.m_index), m_position(rhs.m_position) { }

        ~watermark()
        {
            m_index = -1;
            m_position = -1;
        }

    } watermark_t;

    typedef unsigned char utf8_char;

    /**
     * Calculates the length of a sequence of UTF8 characters.
     *
     * @param utf8_sequence
     * @return
     */
    static std::size_t calculateLengthUtf8(const utf8_char* utf8_sequence);

    utf8_char*          m_buffer;       /// The data-buffer itself
    size_t              m_capacity;     /// The capacity of the buffer
    mutable unsigned    m_hash;         /// The hash code of this string
    size_t              m_length;       /// The length in characters of the buffer
    size_t              m_size;         /// The actual size of the string in bytes
    mutable watermark_t m_watermark;    /// The watermark serve to track the last read position

    scoped_ref<wchar_t> m_wide; /// The wide char* representation of this string

    /**
     * Copies the contents of this string's buffer to the newly specified array.
     * The array must be sufficiently large to hold the resulting copy. These
     * bound checks are not performed, therefore it is programmer's responsibility.
     * <p>
     * <code>startIndex</code> must be an integer larger than zero. If negative,
     * it will be clamped to zero.
     * <p>
     * <code>endIndex</code> must be an integer smaller than the size of the buffer.
     * If larger, it will be clamped to the size of the buffer.
     *
     * @param startIndex
     * @param endIndex
     * @param newArray
     *
     * @return the array passed as parameter
     */
    utf8_char* arrayCopy(int startIndex, int endIndex, utf8_char* newArray, int offset = 0) const;

    /**
     * Checks that the index is a value between 0 (inclusive) and size (exclusive).
     * If the value is not between 0 and size then an IndexOutOfBoundsException
     * is raised.
     * 
     * @param index
     */
    void checkIndexExclusive(int index) const;

    /**
     * Resizes the string to a new capacity. If the string had content copies
     * the content into the newly allocated buffer. The provided integer is
     * a delta, a value to which the capacity is algebraically added.
     * 
     * @param newCapacity
     */
    void ensureCapacity(std::size_t newCapacity);

    /**
     * Constructs (if not constructed yet) and rebuilds the wide character
     * string representation of this string. This is mostly used for 
     */
    void rebuildWideString();

    /**
     * Sets the content of this string from a C string.
     *
     * @param cstr
     */
    void setUtf8FromCString(const char* cstr, const char* const encoding);

    /**
     * Sets the content of this string from a wide character string.
     * 
     * @param wstr
     */
    void setUtf8FromWString(const wchar_t* wstr);

} ;

/**
 * Adds two strings and stores the result in a third string.
 *
 * @param lhs
 * @param rhs
 * @return
 */
inline string operator+(const string& lhs, const string& rhs)
{
    return string(lhs).append(rhs);
}

/**
 * Adds two strings.
 *
 * @param lhs
 * @param rhs
 * @return
 */
inline string operator+(const string& lhs, const char* rhs)
{
    return string(lhs).append(rhs);
}

/**
 * Adds a string to a char.
 *
 * @param lhs
 * @param rhs
 * @return
 */
inline string operator+(const string& lhs, const char rhs)
{
    return string(lhs).append(rhs);
}

}
}

#endif /* STRING_H */

