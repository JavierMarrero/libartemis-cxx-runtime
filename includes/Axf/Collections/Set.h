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
 * File:   Set.h
 * Author: Javier Marrero
 *
 * Created on December 7, 2022, 3:25 PM
 */

#ifndef SET_H
#define SET_H

// API
#include <Axf/Collections/Collection.h>

namespace axf
{
namespace collections
{

/**
 * The set object embodies the mathematical concept of a set: a set is a
 * collection of objects that does not contains duplicates and is considered
 * an object on its own right.
 * <p>
 * The set objects provides the common mathematical set operations, such as
 * union (<code>addAll</code>), intersection (<code>retainAll</code>) and
 * asymmetric difference (<code>removeAll</code>).
 *
 * @author J. Marrero
 */
template <typename E>
class Set : public Collection<E>
{
    AXF_CLASS_TYPE(axf::collections::Set<E>,
                   AXF_TYPE(axf::collections::Collection<E>))

public:


} ;

}
}

#endif /* SET_H */

