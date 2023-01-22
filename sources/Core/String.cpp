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
 * File:   String.cpp
 * Author: Javier Marrero
 * 
 * Created on November 27, 2022, 6:11 PM
 */

#include <Axf/Core/String.h>
#include <Axf/Core/IndexOutOfBoundsException.h>
#include <Axf/Core/IllegalArgumentException.h>
#include <Axf/Core/IllegalStateException.h>
#include <Axf/Core/Object.h>
#include <Axf/Core/Runtime.h>
#include <Axf/Math/BasicMath.h>

// C++
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <cwchar>
#include <cstdarg>

// Libiconv
#include <iconv.h>

using namespace axf;
using namespace axf::core;

string string::format(const char* fmt, ...)
{
    std::size_t sz = std::strlen(fmt);
    scoped_ref<char> b = new char[sz * 0xf];
    fill(b.get(), '\0', sz * 0xf);

    va_list va;
    va_start(va, fmt);
    std::vsprintf(b.get(), fmt, va);
    va_end(va);

    return string(b.get());
}

string::string()
:
m_buffer(NULL),
m_capacity(0),
m_hash(0),
m_length(0),
m_size(0),
m_watermark(0, 0)
{
}

string::string(const string& rhs)
:
m_buffer(rhs.m_buffer),
m_capacity(rhs.m_capacity),
m_hash(rhs.m_hash),
m_length(rhs.m_length),
m_size(rhs.m_size),
m_watermark(rhs.m_watermark),
m_wide(NULL)
{
}

string& string::operator =(const string& rhs)
{
    if (rhs == *this)
        return *this;

    m_buffer = (rhs.m_buffer);
    m_capacity = (rhs.m_capacity);
    m_hash = (rhs.m_hash);
    m_length = (rhs.m_length);
    m_size = (rhs.m_size);
    m_watermark = rhs.m_watermark;

    m_wide.reset();

    return *this;
}

string::string(const char* cstr, const char* charset)
:
m_buffer(NULL),
m_capacity(0),
m_hash(0),
m_length(0),
m_size(0),
m_watermark(0, 0)
{
    setUtf8FromCString(cstr, charset);
}

string::string(const wchar_t* wstr)
:
m_buffer(NULL),
m_capacity(0),
m_hash(0),
m_length(0),
m_size(0),
m_watermark(0, 0)
{
    setUtf8FromWString(wstr);
}

string::string(const uchar& c)
:
m_buffer(NULL),
m_capacity(0),
m_hash(0),
m_length(0),
m_size(0),
m_watermark(0, 0)
{
    char decoded[5] = {0};
    c.decode(io::charset::UTF8_CHARSET, decoded, 4ul);

    setUtf8FromCString(decoded, io::charset::UTF8_CHARSET);
}

string::~string()
{
    clear();
}

string& string::append(const uchar& c)
{
    mutator();

    char utf8_encoded[5] = {0};
    c.decode(axf::io::charset::UTF8_CHARSET, utf8_encoded, 4ul);

    return append(utf8_encoded);
}

string& string::append(const string& str)
{
    mutator();

    ensureCapacity(m_size + str.m_size + 1);
    std::strncat(reinterpret_cast<char*> (m_buffer.get()), reinterpret_cast<const char*> (str.m_buffer.get()), m_size + str.m_size);

    m_size += str.m_size;
    m_length += str.m_length;
    m_buffer.get()[m_size] = '\0';

    return *this;
}

string& string::append(const char* str)
{
    mutator();

    const std::size_t len = std::strlen(str);
    ensureCapacity(m_size + len + 1);
    std::strncat(reinterpret_cast<char*> (m_buffer.get()), str, m_size + len);

    m_size += len;
    m_length += len;

    return *this;
}

string::utf8_char* string::arrayCopy(int startIndex, int endIndex, utf8_char* newArray, int offset /* = 0 */) const
{
    if (m_buffer == NULL)
        return newArray;

    if (startIndex < 0)
        startIndex = 0;
    if (endIndex > (int) m_size)
        endIndex = m_size;

    for (int i = startIndex; i < endIndex; ++i)
    {
        newArray[i] = m_buffer[offset++];
    }

    return newArray;
}

uchar string::at(std::size_t index) const
{
    if (index == m_length)
        return uchar('\0');

    /* Check for index validity */
    checkIndexExclusive(index);

    /*
     * Store a byte pointer to the buffer
     */
    const char* data = reinterpret_cast<const char*> (m_buffer.get());

    /* Find the place we're looking for:
     * 
     * The search begins by calculating the distance to the watermark.
     */
    std::size_t distance = math::max(index, m_watermark.m_index) - math::min(index, m_watermark.m_index);

    /*
     * We have our distance modulo, now we need to see if we run backwards or
     * forward. We run forward if the index is greater than the watermark,
     * backwards otherwise.
     */
    bool forward = (index > m_watermark.m_index);

    /*
     * Set the initial index and position
     */
    std::size_t currentIndex = m_watermark.m_index;
    std::size_t currentPosition = m_watermark.m_position;

    /**
     * Now run a loop, counting characters until we're at the offset.
     */
    std::size_t count = 0;
    while (count != distance)
    {
        /* Update position */
        if (forward)
        {
            currentPosition++;
        }
        else
        {
            currentPosition--;
        }

        char currentByte = *(data + currentPosition);
        if ((currentByte & 0xc0) != 0x80)
        {
            /* We are stepping on a non-continuation byte */
            count++;    // Increase the count of read characters

            if (forward) currentIndex++;
            else currentIndex--;
        }
    }

    /*
     * Store the final offset and position on the watermark
     */
    m_watermark.m_index = currentIndex;
    m_watermark.m_position = currentPosition;

    /* Calculate the length of the sequence */
    std::size_t sequenceLength = 1;
    std::size_t sequencePointer = currentPosition + 1;

    char sequenceByte;
    while (((sequenceByte = *(data + sequencePointer++)) & 0xc0) == 0x80)
    {
        sequenceLength++;
    }

    /* Encode the character object and return */
    return uchar(data + currentPosition, sequenceLength, io::charset::UTF8_CHARSET);
}

std::size_t string::calculateLengthUtf8(const utf8_char* utf8_sequence)
{
    std::size_t utf8Length = 0;
    while (*utf8_sequence != 0u)
    {
        utf8_char byte = *utf8_sequence++;

        if ((byte & 0xc0) != 0x80) utf8Length++;
    }
    return utf8Length;
}

void string::checkIndexExclusive(int index) const
{
    if (index < 0 || index >= ((int) m_size))
    {
        throw IndexOutOfBoundsException("the index is not between 0 and size.", index);
    }
}

void string::clear()
{
    mutator();

    m_buffer.reset();
    m_capacity = 0;
    m_length = 0;
    m_size = 0;
    m_wide.reset();
}

void string::ensureCapacity(std::size_t newCapacity)
{
    if (m_capacity < newCapacity)
    {
        const std::size_t byteSize = m_size;

        m_capacity = newCapacity;
        utf8_char* allocated = new utf8_char[math::max((std::size_t) (m_capacity * 1.4f), newCapacity)];
        std::memset(allocated, 0, m_capacity);

        arrayCopy(0, byteSize, allocated);

        // Deallocate and reassign
        m_buffer = allocated;
    }
}

bool string::equals(const axf::core::Object& rhs) const
{
    return *this == rhs.toString();
}

bool string::equals(const string& rhs) const
{
    if (this == &rhs)
        return true;

    if (rhs.m_buffer == NULL)
        return false;

    if (m_buffer.get() == rhs.m_buffer.get())
        return true;

    return std::strcmp(reinterpret_cast<const char*> (m_buffer.get()), reinterpret_cast<const char*> (rhs.m_buffer.get())) == 0;
}

unsigned int string::hash() const
{
    if (m_buffer && m_hash == 0)
    {
        const char* data = reinterpret_cast<const char*> (m_buffer.get());
        int c;

        while ((c = *data++) != '\0')
        {
            m_hash = c + (m_hash << 6) + (m_hash << 16) - m_hash;
        }
    }
    return m_hash;
}

std::size_t string::indexOf(const uchar c) const
{
    for (std::size_t idx = 0; idx < m_length; ++idx)
    {
        if (at(idx) == c)
            return idx;
    }
    return NPOS;
}

std::size_t string::lastIndexOf(const uchar c) const
{
    std::size_t count = m_length;
    while (count-- > 0)
    {
        uchar uc = at(count - 1);
        if (uc == c)
        {
            return count - 1;
        }
    }
    return NPOS;
}

void string::mutator() noexcept
{
    /*
     * The string only gets copied if there are more than one reference to the
     * string.
     */
    if (m_buffer.users() > 1)
    {
        utf8_char* buffer = new utf8_char[m_capacity];
        std::memcpy(buffer, m_buffer.get(), m_capacity);

        m_buffer = buffer;

        // Clear the wide character buffer
        m_wide.reset();
    }
}

string string::substring(std::size_t start, std::size_t end) const
{
    // Bounds check
    if (start >= m_length || (end >= m_length && end != NPOS) || (start >= end))
        throw IllegalArgumentException("illegal substring operation [%z:%z]", start, end);

    at(start);  // Move the watermark
    std::size_t byteOffsetStart = m_watermark.m_position;
    std::size_t byteOffsetEnd = (end == NPOS) ? m_size : (at(end), m_watermark.m_position);
    std::size_t length = byteOffsetEnd - byteOffsetStart;

    scoped_ref<char> buffer = new char[length + 1];
    buffer.get()[length] = '\0';

    const char* bytes = this->bytes();
    std::memcpy(buffer.get(), bytes + byteOffsetStart, length);

    return string(buffer.get());
}

// ICONV FUNCTIONS

namespace _iconv
{

static inline iconv_t open(const char* to, const char* from)
{
    iconv_t iv = iconv_open(to, from);
    if (iv == (iconv_t) (-1))
    {
        throw IllegalStateException("unable to allocate conversion descriptor.");
    }

    return iv;
}

static inline void convert(iconv_t& iv, const void* in, char* out, const std::size_t inl, const std::size_t outl)
{
    char* inbuf = reinterpret_cast<char*> (const_cast<void*> (in));
    char* outbuf = out;

    std::size_t in_size = inl;
    std::size_t out_size = outl;

    std::size_t result = iconv(iv, &inbuf, &in_size, &outbuf, &out_size);
    iconv_close(iv);

    if (result == (size_t) (-1))
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

}
// ICONV -*-*-*-*-

void string::rebuildWideString() const
{
    if (m_wide.isNull())
    {
        iconv_t iv = _iconv::open("wchar_t//TRANSLIT", "UTF-8");

        std::size_t expectedLength = m_length * sizeof (wchar_t);
        scoped_ref<wchar_t, ws_deleter> memory = new wchar_t[m_length + 1];
        core::fill(memory.get(), L'\0', m_length + 1);

        _iconv::convert(iv, m_buffer.get(), reinterpret_cast<char*> (memory.get()), m_size, expectedLength);

        // Assign it
        m_wide = memory;
    }
}

void string::setUtf8FromCString(const char* cstr, const char* const encoding)
{
    std::size_t memorySize = std::strlen(cstr);
    std::size_t expectedSize = memorySize;

    if (encoding == axf::io::charset::UTF8_CHARSET)
    {
        /*
         * If we're inside this, the string is already encoded in UTF-8.
         */
        ensureCapacity(memorySize + 1);
    }
    else
    {
        /*
         * We're encoding in UTF-8 assuming the passed array of bytes is
         * correctly encoded into some well defined character set. Conversion
         * will fail otherwise.
         */

        // Size may be trimmed latter
        std::size_t bytesPerCharacter = sizeof (unsigned int);  // A conservatively chosen behavior
        if (encoding == axf::io::charset::ASCII_CHARSET
            || encoding == axf::io::charset::ISO_8859_1)
        {
            bytesPerCharacter = 1;
        }
        else if (encoding == axf::io::charset::UTF16_CHARSET)
        {
            bytesPerCharacter = 2;
        }
        else if (encoding == axf::io::charset::UTF32_CHARSET)
        {
            bytesPerCharacter = 4;
        }

        ensureCapacity((memorySize + 1) * bytesPerCharacter);
        expectedSize = memorySize * bytesPerCharacter;
    }

    iconv_t iv = _iconv::open("UTF-8//TRANSLIT", encoding);
    _iconv::convert(iv, cstr, reinterpret_cast<char*> (m_buffer.get()), memorySize, expectedSize);

    // Basic properties
    m_length = calculateLengthUtf8(m_buffer.get());
    m_size = std::strlen(reinterpret_cast<const char*> (m_buffer.get()));

    // Rebuild the w-char string
    rebuildWideString();
}

void string::setUtf8FromWString(const wchar_t* wstr)
{
    iconv_t iv = _iconv::open("UTF-8//TRANSLIT", "wchar_t");

    std::size_t expectedLength = std::wcslen(wstr);
    ensureCapacity(expectedLength + 1);

    _iconv::convert(iv, wstr, reinterpret_cast<char*> (m_buffer.get()), expectedLength * sizeof (wchar_t), expectedLength * 6);

    // Set some basic props
    m_length = calculateLengthUtf8(m_buffer.get());
    m_size = std::strlen(reinterpret_cast<const char*> (m_buffer.get()));

    // rebuild the wide char string
    scoped_ref<wchar_t, ws_deleter> memory = new wchar_t[(expectedLength + 1) * sizeof (wchar_t)];
    memory.get()[m_length] = L'\0';

    m_wide = memory;
    std::memcpy(m_wide.get(), wstr, expectedLength * sizeof (wchar_t));
}
