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
 * File:   BasicIterator.h
 * Author: Javier Marrero
 *
 * Created on December 4, 2022, 8:43 PM
 */

#ifndef ITERATOR_H
#define ITERATOR_H

// API
#include <Axf/Core/Object.h>
#include <Axf/Core/Memory.h>

namespace axf
{
namespace collections
{

/**
 * BasicIterators are conceptually defined as being abstract pointers to some point
 * in the sequence.
 * <p>
 * This concept differs from the classical <i>Java</i> iterator and is more
 * idiomatic to C++.
 *
 * @author J. Marrero
 */
template <typename E>
class BasicIterator : public core::Object
{
public:

    /**
     * Returns a reference to the currently pointed object.
     *
     * @return
     */
    virtual E& current() = 0;

    virtual const E& current() const = 0;

    /**
     * Returns a reference to the currently pointed object and advances the
     * iterator one position forward.
     * 
     * @return
     */
    virtual E& next() = 0;

    virtual const E& next() const = 0;

    /**
     * Dereferences the iterator.
     * 
     * @return
     */
    inline E& operator*()
    {
        return current();
    }

    inline const E& operator*() const
    {
        return current();
    }

    /**
     * Advances the iterator one position to the right in the sequence.
     *
     * @return
     */
    inline BasicIterator<E>& operator++()
    {
        next();
        return *this;
    }

    /**
     * Compares two iterators. Two iterators are equals if their pointed objects
     * matches.
     * 
     * @param rhs
     * @return
     */
    inline bool operator==(const BasicIterator<E>& rhs) const
    {
        return equals(rhs);
    }

    /**
     * Compares two iterators. Two iterators are deemed different if they do
     * not point to a valid object.
     * 
     * @param rhs
     * @return
     */
    inline bool operator!=(const BasicIterator<E>& rhs) const
    {
        return equals(rhs) == false;
    }

protected:

    static const std::size_t BAD_POINTER = 0xbad0bad0;

    inline static void* getBadPointer()
    {
        return (void*) (BAD_POINTER);
    }
} ;

/**
 * Originally, the iterator methods in the collection classes returned a full
 * fledged scoped_ptr to an iterator. This was deemed not necessary and also
 * complicated the iterator declaration syntax.
 * <p>
 * This class solves the aforementioned issues, by providing an interface
 * to the iterator class and also managing the allocation/deallocation of
 * the iterator itself.
 *
 * @author J. Marrero
 */
template <typename T>
class iterator
{
public:

    iterator() : m_iterator(NULL) { }

    iterator(BasicIterator<T>* iterator) : m_iterator(iterator) { }

    iterator(const iterator<T>& rhs) : m_iterator(rhs.m_iterator) { }

    ~iterator() { }

    /**
     * Default implicit cast conversion operator
     *
     * @return
     */
    inline operator BasicIterator<T>&()
    {
        return *m_iterator;
    }

    inline operator BasicIterator<T>&() const
    {
        return *m_iterator;
    }

    inline T& operator*()
    {
        return m_iterator->current();
    }

    inline const T& operator*() const
    {
        return m_iterator->current();
    }

    inline BasicIterator<T>* operator->()
    {
        return m_iterator.get();
    }

    inline const BasicIterator<T>* operator->() const
    {
        return m_iterator.get();
    }

    bool operator!=(const iterator<T>& rhs) const
    {
        return *m_iterator != *rhs.m_iterator;
    }

    inline bool operator==(const iterator<T>& rhs) const
    {
        return *m_iterator == *rhs.m_iterator;
    }

    inline iterator<T>& operator++()
    {
        m_iterator->next();
        return *this;
    }

    inline iterator<T> operator++(int) const
    {
        iterator<T> temp = *this;
        m_iterator->next();

        return temp;
    }

private:

    mutable core::strong_ref<BasicIterator<T> > m_iterator;
} ;



}
}

#endif /* ITERATOR_H */

