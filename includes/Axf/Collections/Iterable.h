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
 * File:   Iterable.h
 * Author: Javier Marrero
 *
 * Created on December 5, 2022, 1:53 AM
 */

#ifndef ITERABLE_H
#define ITERABLE_H

// API
#include <Axf/Collections/Iterator.h>

namespace axf
{
namespace collections
{

/**
 * The <code>Iterable</code> interface describes collections that can be
 * iterated through. This interface decouples the <i>iterable</i> property
 * from the <i>collection type</i>
 * <p>
 * 
 *
 * @author J. Marrero
 */
template <typename T>
class Iterable : virtual public core::Object
{
    AXF_CLASS_TYPE(axf::collections::Iterable<T>,
                   AXF_TYPE(axf::core::Object))
public:

    /**
     * Returns an iterator to the beginning of this sequence.
     *
     * @return
     */
    virtual iterator<T> begin() = 0;

    virtual const iterator<T> begin() const = 0;

    /**
     * Returns an iterator to the end of this sequence. The returned iterator
     * does not points to a valid list element.
     *
     * @return
     */
    virtual iterator<T> end() = 0;

    virtual const iterator<T> end() const = 0;
} ;

}
}

#endif /* ITERABLE_H */

