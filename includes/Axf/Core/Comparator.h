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
 * File:   Comparator.h
 * Author: Javier Marrero
 *
 * Created on December 12, 2022, 10:35 PM
 */

#ifndef COMPARATOR_H
#define COMPARATOR_H

// API
#include <Axf/Core/Object.h>

namespace axf
{
namespace core
{

/**
 * Defines a comparator function-like object that takes two arguments and
 * returns the result of comparing two objects. Comparison may be performed
 * based on arbitrary constraints.
 * <p>
 * The specified comparator must return values so that when comparing to zero
 * all the operators express their meaning in a correct way: they must be
 * semantically consistent.
 * <p>
 * For example, the most common return values are -1, 0 and 1. This means the
 * following:
 * <pre>
 * Given two objects, A and B, if A > B will return 1,<br/>
 * A == B will return 0 and A &lt B must return -1.
 * </pre>
 * <p>
 * This class has the () operator overloaded as specified for functional
 * interfaces.
 *
 * @author J. Marrero
 */
template <class T>
class Comparator : virtual public Object
{
    AXF_CLASS_TYPE(axf::core::Comparator<T>,
                   AXF_TYPE(axf::core::Object))

public:

    /**
     * Performs the actual comparison.
     * 
     * @see this class´ description.
     */
    virtual int compare(const T& obj1, const T& obj2) const = 0;

    /**
     * Default overload for the () operator, as specified in the functional
     * interface requirements.
     *
     * @param a
     * @param b
     * @return
     */
    inline int operator()(const T& a, const T& b) const
    {
        return compare(a, b);
    }
} ;

}
}

#endif /* COMPARATOR_H */

