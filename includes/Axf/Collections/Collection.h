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
#include <Axf/Collections/Iterable.h>
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
class Collection : virtual public core::Object, virtual public Iterable<E>
{
    AXF_CLASS_TYPE(axf::collections::Collection<E>,
                   AXF_TYPE(axf::core::Object),
                   AXF_TYPE(axf::collections::Iterable<E>))
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
     * Clears the contents of this collection. The collection should end with
     * zero size. Depending on the particular implementation it can actually
     * release memory or not.
     *
     * @return
     */
    virtual void clear() = 0;

    /**
     * Returns true if this collection contains the specified value.
     *
     * @param element
     * @return
     */
    virtual bool contains(const E& element) const = 0;

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

