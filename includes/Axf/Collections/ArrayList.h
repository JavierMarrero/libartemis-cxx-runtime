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
 * File:   ArrayList.h
 * Author: Javier Marrero
 *
 * Created on December 7, 2022, 9:40 PM
 */

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

// API
#include <Axf/Collections/Collection.h>
#include <Axf/Collections/DefaultAllocator.h>
#include <Axf/Collections/List.h>
#include <Axf/Core/IndexOutOfBoundsException.h>
#include <Axf/Core/Runtime.h>
#include <Axf/Math/BasicMath.h>

// C++
#include <cassert>
#include <cstring>

namespace axf
{
namespace collections
{
namespace bits
{

/**
 * An iterator that enables forward iteration through an array list.
 *
 * @return
 */
template <typename E>
class ArrayListIterator : public BasicIterator<E>
{

    AXF_CLASS_TYPE(axf::collections::bits::ArrayListIterator<E>,
                   AXF_TYPE(axf::collections::BasicIterator<E>))

public:


    ArrayListIterator(E* array, std::size_t index) :
    m_array(array), m_index(index) { }

    ~ArrayListIterator() { }

    inline virtual const E& current() const
    {
        return m_array[m_index];
    }

    inline virtual E& current()
    {
        return m_array[m_index];
    }

    virtual E& next()
    {
        E& result = current();
        m_index++;

        return result;
    }

    virtual const E& next() const
    {
        const E& result = current();
        m_index++;

        return result;
    }

    inline virtual bool equals(const core::Object& object) const
    {
        const ArrayListIterator<E>& rhs = static_cast<const ArrayListIterator<E>&> (object);
        return m_index == rhs.m_index;
    }

private:

    E*                  m_array;
    mutable std::size_t m_index;

} ;

}

/**
 * Array lists are array backed implementations of the <code>List</code>
 * interface.
 * <p>
 * This list implements all the optional <code>List</code> operations and
 * when used with pointers allows <code>null</code> pointers. Random access
 * is roughly constant time, and iteration is roughly linear time. As so is
 * insertion and deletion, which run in amortized linear time.
 * <p>
 * Each list has a capacity, and whenever that capacity is reached a new array
 * is allocated and its contents transferred. One can also pre-reserve space
 * with the <code>reserve</code> and <code>trim</code> operations.
 * <p>
 * Array lists are not synchronized.
 * 
 * @author J. Marrero
 */
template <typename E, typename allocator_t = axf::collections::DefaultAllocator<E> >
class ArrayList : public List<E>
{

    AXF_CLASS_TYPE(AXF_TEMPLATE_CLASS(axf::collections::ArrayList<E>),
                   AXF_TYPE(axf::collections::Collection<E>))
public:

    ArrayList(std::size_t capacity = 10)
    :
    m_array(NULL), m_capacity(capacity), m_size(0)
    {
        // Allocate the list
        m_array = m_allocator.newArray(capacity);
    }

    ArrayList(const ArrayList<E>& rhs)
    :
    m_array(NULL), m_capacity(rhs.m_capacity), m_size(rhs.m_size)
    {
        // Allocate the list
        m_array = m_allocator.newArray(m_capacity);

        // Copy the array
        core::arrayCopy(m_array, 0, rhs.m_array, 0, rhs.m_size);
    }

    ~ArrayList()
    {
        assert(m_array != NULL && "how come the array got to be null?");

        // Remove the array
        m_allocator.deleteArray(m_array, m_size);

        // Set the capacity and size to invalid values
        m_capacity = 0;
        m_size = 0;
    }

    virtual bool add(std::size_t index, const E& data)
    {
        checkBoundsInclusive(index);

        if (m_size == m_capacity)
            reserve(m_size + 1);

        if (index != m_size)
        {
            std::memmove(reinterpret_cast<void*> (m_array + index + 1), reinterpret_cast<const void*> (m_array + index), (m_size - index) * sizeof (E));
        }
        new (&m_array[index]) E(data);
        m_size++;

        return true;
    }

    virtual bool add(const E& element)
    {
        return add(m_size, element);
    }

    virtual const iterator<E> begin() const
    {
        if (m_size == 0)
            return end();
        return new bits::ArrayListIterator<E>(m_array, 0);
    }

    virtual iterator<E> begin()
    {
        if (m_size == 0)
            return end();
        return new bits::ArrayListIterator<E>(m_array, 0);
    }

    inline void clear()
    {
        if (m_size > 0)
        {
            m_allocator.destruct(m_array, m_size);
            core::memset(m_array, 0, m_size);
            m_size = 0;
        }
    }

    inline virtual bool contains(const E& element) const
    {
        return indexOf(element) != -1;
    }

    virtual const iterator<E> end() const
    {
        return new bits::ArrayListIterator<E>(m_array, m_size);
    }

    virtual iterator<E> end()
    {
        return new bits::ArrayListIterator<E>(m_array, m_size);
    }

    virtual E& get(std::size_t index)
    {
        checkBoundsExclusive(index);

        return m_array[index];
    }

    virtual const E& get(std::size_t index) const
    {
        checkBoundsExclusive(index);

        return m_array[index];
    }

    long indexOf(const E& element) const
    {
        for (unsigned i = 0; i < m_size; ++i)
        {
            if (element == m_array[i])
                return i;
        }
        return -1;
    }

    inline virtual bool isEmpty() const
    {
        return m_size == 0;
    }

    long lastIndexOf(const E& element) const
    {
        for (unsigned i = m_size - 1; i >= 0; --i)
        {
            if (element == m_array[i])
                return i;
        }
        return -1;
    }

    virtual bool remove(const E& element)
    {
        for (std::size_t i = 0; i < m_size; ++i)
        {
            if (element == m_array[i])
            {
                removeAt(i);
                return true;
            }
        }
        return false;
    }

    virtual bool removeAt(std::size_t index)
    {
        checkBoundsExclusive(index);

        if (index != --m_size)
        {
            std::memmove(reinterpret_cast<void*> (m_array + index), reinterpret_cast<const void*> (m_array + index + 1), (m_size - index) * sizeof (E));
        }
        return true;
    }

    /**
     * Guarantees the list will have at least enough capacity to hold
     * <i>minCapacity</i> elements. This implementation will choose between
     * a reduced capacity model and duplicate the size of the list.
     *
     * @param minCapacity the minimum guaranteed capacity
     */
    inline void reserve(std::size_t minCapacity)
    {
        std::size_t current = m_capacity;
        std::size_t final = math::max(current * 2, minCapacity);

        if (minCapacity > current)
        {
            E* newData = m_allocator.newArray(final);
            core::arrayCopy(newData, 0, m_array, 0, m_size);

            m_allocator.deleteArray(m_array, m_size);
            m_array = newData;
            m_capacity = final;
        }
    }

    inline virtual size_t size() const
    {
        return m_size;
    }

    /**
     * Trims the capacity of the list to match exactly its size. A memory saver.
     */
    inline void trim()
    {
        if (m_size != m_capacity)
        {
            E* newData = m_allocator.newArray(m_size);
            core::arrayCopy(newData, 0, m_array, 0, m_size);

            m_allocator.deleteArray(m_array, m_capacity);
            m_array = newData;
            m_capacity = m_size;
        }
    }

private:

    allocator_t m_allocator;
    E*          m_array;
    std::size_t m_capacity;
    std::size_t m_size;

    /**
     * Checks that the index is in the range of possible elements.
     *
     * @param index
     */
    inline void checkBoundsInclusive(std::size_t index) const
    {
        if (index > m_size)
        {
            raiseBoundsError(index);
        }
    }

    /**
     * Checks that the index is in the range of existing elements.
     *
     * @param index
     */
    inline void checkBoundsExclusive(std::size_t index) const
    {
        if (index >= m_size)
        {
            raiseBoundsError(index);
        }
    }

    /**
     * Raises an <code>IndexOutOfBoundsExceptions</code>.
     *
     * @param index
     */
    inline void raiseBoundsError(std::size_t index) const
    {
        throw core::IndexOutOfBoundsException("performed operation over array list with illegal index", index);
    }
} ;

}
}

#endif /* ARRAYLIST_H */

