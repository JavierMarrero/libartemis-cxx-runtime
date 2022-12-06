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
 * File:   List.h
 * Author: Javier Marrero
 *
 * Created on December 4, 2022, 12:38 PM
 */

#ifndef LIST_H
#define LIST_H

// API
#include <Axf/Collections/Collection.h>

namespace axf
{
namespace collections
{

template <typename E>
class List : public Collection<E>
{
    AXF_CLASS_TYPE(axf::collections::List<E>,
               AXF_TYPE(axf::collections::Collection<E>))
public:

    /**
     * Adds the element at the specified index.
     *
     * @param index
     * @param data
     * @return
     */
    virtual bool add(std::size_t index, const E& data) = 0;

    /**
     * Const version of the get by index method.
     *
     * @param index
     * @return
     */
    virtual const E& get(std::size_t index) const = 0;

    /**
     * Non-const version of the get by index method. It returns a reference to
     * the element placed at the supplied index. If the index is incorrect
     * returns an index out of bounds exception.
     *
     * @param index
     * @return
     */
    virtual E& get(std::size_t index) = 0;

    /**
     * Removes the element at the index provided in this list.
     *
     * @param index
     * @return
     */
    virtual bool removeAt(std::size_t index) = 0;
} ;

}
}

#endif /* LIST_H */

