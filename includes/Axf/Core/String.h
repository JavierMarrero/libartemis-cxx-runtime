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
#include <Axf/Core/Lang-C++/keywords.h>
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
 * </p>
 * <h2>Observations on the memory model for strings in the Artemis Framework</h2>
 * <p>
 * Since the beginning of the design process of the <i>Artemis Framework</i>,
 * great emphasis has been made into designing an efficient <code>string</code>
 * type that be efficient both in time and space, as much as possible.
 * <p>
 * For understanding what is the main design challenge for achieving efficient
 * C++ strings, one must understand that there are, fundamentally, two types of
 * <code>string</code> type implementation:
 * <ul>
 *  <li>Mutable strings</li>
 *  <li>Immutable strings</li>
 * </ul>
 * <p>
 * <b>Mutable strings</b> provides great flexibility; since there's no need for
 * returning a new object every time a <b><i>mutative operation</b></i> is
 * performed on string objects. However, this implies that there can't be copies
 * of the same string object across the code. Every time we do (or C++ does) a
 * copy of the string, the copy must be deep: dynamic memory must be allocated
 * (which is a slow operation), capacities must be updated (which may be an
 * operation in the order of <i>O<sub>(2n)</sub></i>, etc...
 * <p>
 * <b>Immutable strings</b> provides less flexibility at the expense of some
 * nifty benefits. Immutable string objects are thread safe, the memory consumption
 * is minor (since one does not have to deep copy the string every time) and
 * comparisons are sped up (since it is possible to do a pointer comparison which
 * most of the time is <i>O<sub>(1)</sub></i>.
 * <p>
 * Since in <i>C++</i> it is possible to distinguish from <b>mutative</b> and
 * <i>observer</i> methods, a model can be achieved that combines the advantages
 * of both mutable and immutable strings. This can be achieved using reference
 * counting and a smart use of the <b>const correctness</b> in the C++ language.
 * </p>
 * <h3>Implementation</h3>
 * <p>
 * For this particular implementation we will be using smart reference counting
 * coupled with the fact that the class implementors know what are the mutative
 * methods.
 * <p>
 * Basically, when a copy of a string object is created, the buffer is shared
 * between the two objects. Nothing else is shared <i>per se</i>, just the
 * buffer that contains the data; though the capacity, length and size will
 * remain the same. The watermark can vary since the at operation can be safely
 * performed on the same object as long as the waterfall is a different object,
 * which is guaranteed by the copy constructor of the string object itself and
 * the concept of object composition in C++.
 * <p>
 * The really nifty thing is when one performs a mutative operation on the
 * string type. Then the <code>mutator</code> method is called, which actually
 * performs a deep copy of the string buffer and its contents. Once a deep copy
 * is performed, as long as the number of references to the buffer is equals to
 * one, the string can be mutated safely without the need to call the
 * <code>mutator</code> signaling method.
 * <p>
 * <h3>And what happens with wide strings?</h3>
 * <p>
 * Wide strings are not shared between string objects, and they are lazily
 * generated.
 *
 * @author J. Marrero
 */
class string : protected ReferenceCounted
{
private:

    /**
     * Deletes a wide character buffer in the form of a wide character array.
     *
     * @param pointer
     */
    struct ws_deleter
    {

        /**
         * Overload of the () operator to make this look as a functor.
         * 
         * @param pointer
         */
        inline void operator ()(wchar_t* pointer)
        {
            delete[] pointer;
        }
    } ;

    typedef unsigned char utf8_char;

    /**
     * Used as a deleter of the <code>strong_ptr</code>.
     */
    struct utf8c_deleter
    {

        inline void operator()(utf8_char* pointer)
        {
            delete[] pointer;
        }
    } ;

public:

    /**
     * An integer value representing an invalid position in the string. It is
     * normally the return value of methods that need to find or return an index
     * and the index returned is not valid somehow.
     */
    static const std::size_t NPOS = -1;

    /**
     * This method formats a string via the standard 'printf' format syntax and
     * mechanism. The <code>format</code> operation takes a raw variable number
     * of arguments roughly equivalent to their <code>printf</code> equivalents,
     * therefore, this function is not considered <i>safe</i> since it doesn't
     * account for the common errors that one may encounter when using 'printf'.
     * <p>
     * However, for the regular day, this function is just enough; and in a
     * controlled environment it shouldn't pose any security threat.
     * <p>
     * For a more controlled and safe format utility (and perhaps, even more
     * powerful, in dependence of your definition of powerful) use the
     * <code>format</code> package utilities.
     *
     * @param fmt
     * @param ...
     * @return
     */
    static string format(const char* fmt, ...);

    string();                       /// Default constructor
    string(const string& rhs);      /// Default copy constructor
    ~string();                      /// This class' destructor is not marked virtual on purpose

    string(const char* cstr, const char* charset = axf::io::charset::UTF8_CHARSET); /// Constructs a string via a pointer to a c string and a charset
    string(const wchar_t* wstr);                                                    /// Constructs a string via a wide character array
    string(const uchar& c);

    /**
     * Appends the given unicode character to the given string. This is a mutator
     * method.
     *
     * @param c
     * @return
     */
    string& append(const uchar& c);

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
        // Rebuilds the wide string, which is now a non mutative operation
        // and allows lazy construction of the wide string
        rebuildWideString();

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
        return reinterpret_cast<const char*> (m_buffer.get());
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
     * Returns the first index of the provided character in the string. It
     * performs linear search. If the character is not on the string then
     * <code>NPOS</code> is returned.
     *
     * @see lastIndexOf
     * @param c
     * @return
     */
    std::size_t indexOf(const uchar c) const;

    /**
     * Returns the last index of a character in the string. It performs a
     * reversed linear search so the complexity is <code>O<sub>(n)</sub></code>.
     * <p>
     * If the character is not found on the string, then <code>NPOS</code> is
     * returned (the equivalent <code>std::size_t</code> of the -1 value).
     * 
     * @param c
     * @return
     */
    std::size_t lastIndexOf(const uchar c) const;

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
     * Returns a <b>new</b> <code>string</code> object that is an exact
     * substring of this, with <code>start</code> and <code>end</code>
     * as limit indexes.
     * <p>
     * As assertions of this function, it is required that <code>end is either</code>
     * <code>NPOS</code> or a positive integer greater than <code>start</code>,
     * and <code>start</code> can't be greater than <code>end</code>.
     * <p>
     * If <code>NPOS</code> is provided as the <code>end</code> parameter,
     * the end of the string is assumed.
     * <p>
     * <b>Note</b>: If invalid limits are provided, exceptions will be thrown.
     *
     * @param start
     * @param end
     * @return
     */
    string substring(std::size_t start = 0, std::size_t end = NPOS) const;

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
    string& operator=(const string& rhs);

    /**
     * Compares two strings for equality.
     *
     * @param rhs
     * @return
     */
    inline bool operator==(const string& rhs) const
    {
        return equals(rhs);
    }

    /**
     * Compares a string and a C string for equality.
     *
     * @param cstr
     * @return
     */
    inline bool operator==(const char* cstr) const
    {
        return std::strcmp(reinterpret_cast<const char*> (m_buffer.get()), cstr) == 0;
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

    /**
     * Calculates the length of a sequence of UTF8 characters.
     *
     * @param utf8_sequence
     * @return
     */
    static std::size_t calculateLengthUtf8(const utf8_char* utf8_sequence);

    strong_ref<utf8_char, utf8c_deleter>    m_buffer;       /// The data-buffer itself
    size_t                                  m_capacity;     /// The capacity of the buffer
    mutable unsigned                        m_hash;         /// The hash code of this string
    size_t                                  m_length;       /// The length in characters of the buffer
    size_t                                  m_size;         /// The actual size of the string in bytes
    mutable watermark_t                     m_watermark;    /// The watermark serve to track the last read position

    mutable scoped_ref<wchar_t, ws_deleter> m_wide; /// The wide char* representation of this string

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
     * Signals that a mutative operation will be performed.
     */
    void mutator() noexcept;

    /**
     * Constructs (if not constructed yet) and rebuilds the wide character
     * string representation of this string. This is mostly used for 
     */
    void rebuildWideString() const;

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

}
}

#endif /* STRING_H */

