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
 * File:   Collection.h
 * Author: Javier Marrero
 *
 * Created on December 4, 2022, 12:39 PM
 */

#ifndef COLLECTION_H
#define COLLECTION_H

// API
#include <Axf/Collections/Iterator.h>
#include <Axf/Core/Memory.h>
#include <Axf/Core/Object.h>

namespace axf
{
namespace collections
{

/**
 * The <code>Collection</code> interface is the center of the <i>Artemis
 * Collection Framework</i>.
 *
 * @return
 */
template <typename E>
class Collection : virtual public core::Object
{
    AXF_CLASS_TYPE(axf::collections::Collection<E>,
                   AXF_TYPE(axf::core::Object))
public:

    /**
     * Adds the element to the collection.
     * <p>
     * Placement is unspecified, generally, the insertion is performed at the
     * most efficient place.
     *
     * @param element
     * @return
     */
    virtual bool add(const E& element) = 0;

    /**
     * Returns an iterator to the beginning of this sequence.
     *
     * @return
     */
    virtual iterator_ref<E> begin() = 0;

    /**
     * Returns an iterator to the end of this sequence. The returned iterator
     * does not points to a valid list element.
     *
     * @return
     */
    virtual iterator_ref<E> end() = 0;

    /**
     * Returns true if the collection is empty.
     *
     * @return
     */
    virtual bool isEmpty() const = 0;

    /**
     * Removes the first occurrence of this element from the collection.
     * 
     * @param element
     * @return
     */
    virtual bool remove(const E& element) = 0;

    /**
     * Returns the size of this collection.
     *
     * @return
     */
    virtual std::size_t size() const = 0;

} ;

}
}

#endif /* COLLECTION_H */

