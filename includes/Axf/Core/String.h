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

// API
#include <Axf/Core/ReferenceCounted.h>

namespace axf
{
namespace core
{

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
    ~string();                      /// This class' destructor is not marked virtual on purpose

    string(const char* cstr);       /// Constructs a string via a pointer to a c string
    string(const wchar_t* wstr);    /// Constructs a string via a wide character array

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
     * Returns the bytes of this string as a pointer to char.
     * 
     * @return
     */
    inline const char* bytes() const
    {
        return m_buffer;
    }

    /**
     * Clear this string's content, releasing all the memory allocated by this
     * object. Memory will be re-allocated if deemed necessary (when mutating
     * this string).
     */
    void clear();

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

private:

    typedef char utf8_char;

    utf8_char*  m_buffer;       /// The data-buffer itself
    size_t      m_capacity;     /// The capacity of the buffer
    size_t      m_length;       /// The length in characters of the buffer
    size_t      m_size;         /// The actual size of the string in bytes

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
    utf8_char* arrayCopy(int startIndex, int endIndex, utf8_char* newArray) const;

    /**
     * Checks that the index is a value between 0 (inclusive) and size (exclusive).
     * If the value is not between 0 and size then an IndexOutOfBoundsException
     * is raised.
     * 
     * @param index
     */
    void checkIndexExclusive(int index);

    /**
     * Resizes the string to a new capacity. If the string had content copies
     * the content into the newly allocated buffer. The provided integer is
     * a delta, a value to which the capacity is algebraically added.
     * 
     * @param newCapacity
     */
    void resize(int delta);

} ;

}
}

#endif /* STRING_H */

