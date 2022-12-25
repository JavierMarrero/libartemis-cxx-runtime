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
 * File:   Runtime.h
 * Author: Javier Marrero
 *
 * Created on December 9, 2022, 3:16 PM
 */

#ifndef RUNTIME_H
#define RUNTIME_H

// API
#include <Axf/Core/Object.h>
#include <Axf/Core/IndexOutOfBoundsException.h>

// C++
#include <cstddef>
#include <cstring>

namespace axf
{
namespace core
{

/**
 * Copies the content of the array 'source' into the array 'destination' in the
 * boundaries specified by the start and end ranges.
 * <p>
 * The destination array must have enough space to store the specified range,
 * else segmentation failure may arise. Since this function takes as parameter
 * raw arrays there's no way of checking boundaries. Special care must be taken.
 * You might prefer to use one of the built-in array classes that this library
 * puts at your disposal.
 * <p>
 * The function returns the number of elements actually copied into the
 * destination array.
 *
 * @param destination
 * @param offset
 * @param source
 * @param start
 * @param end
 * 
 * @return
 */
template <typename T>
std::size_t arrayCopy(T* destination, std::size_t offset, const T* source, std::size_t start, std::size_t end)
{
    if (end < start)
        throw IndexOutOfBoundsException("attempted to call arrayCopy with the end lesser than the start offset", end);

    T* destElement = static_cast<T*> (destination + offset);
    std::size_t current = start;
    std::size_t copied = 0;

    while (current != end)
    {
        *(destElement++) = *(source + current++);
        copied++;
    }
    return copied;
}

/**
 * Fills the provided array with the contents of the passed value. This
 * function reinterprets the bits within the memory block, therefore, zero
 * does not necessarily means the numerical zero.
 *
 * @param memory
 * @param value
 * @param size
 * @return
 */
template <typename T>
T* memset(T* memory, int value, std::size_t size)
{
    return static_cast<T*> (std::memset(reinterpret_cast<void*> (memory), value, size * sizeof (T)));
}

/**
 * Fills the provided array with the value of the passed object parameter,
 * up to count objects. The array must contain objects assignable with the
 * default assignment (<code>=</code>) operator.
 *
 * @param memory
 * @param value
 * @param count
 * @return
 */
template <typename T>
std::size_t fill(T* memory, const T& value, std::size_t count)
{
    std::size_t i;
    for (i = 0; i < count; ++i)
    {
        memory[i] = value;
    }
    return i;
}

/**
 * The <code>Runtime</code> class encapsulates some methods and properties of
 * the current runtime system. It allows to know the properties of the system
 * one's running in.
 * <p>
 * The class also performs some initialization and de-initialization that is
 * essential to the framework.
 *
 * @author J. Marrero
 */
class Runtime : public Object
{

    AXF_CLASS_TYPE(axf::core::Runtime,
                   AXF_TYPE(axf::core::Object))

public:

    typedef enum _SystemType
    {
        MS_WINDOWS,
        MACOS,
        GNULINUX,
        BSD,
        GENERIC_UNIX,
        UNKNOWN_OS
    } SystemType;

    typedef enum _Endianness
    {
        LITTLE_ENDIAN,
        BIG_ENDIAN
    } Endianness;

    /**
     * Returns the enumerated identifier for this machine's operating system.
     *
     * @return
     */
    static const SystemType getCurrentPlatform();

    /**
     * Returns the system endianness as an enumerated type.
     *
     * @return
     */
    static const Endianness getSystemEndianness();

    /**
     * Performs a byte swap of a 16-bit number, this effectively modifies
     * endianness.
     *
     * @param value
     * @return
     */
    static unsigned short swapEndianness(unsigned short value);

    /**
     * Swaps the endianness of a 32-bit integer number.
     *
     * @param value
     * @return
     */
    static unsigned int swapEndianness(unsigned int value);

private:

    class StaticRuntimeInitializer
    {
        friend class Runtime;

        StaticRuntimeInitializer();
        ~StaticRuntimeInitializer();
    } ;

    static StaticRuntimeInitializer _srinit;

    Runtime() { };

    ~Runtime() { }

} ;

}
}

#endif /* RUNTIME_H */

