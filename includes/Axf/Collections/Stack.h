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
 * File:   Stack.h
 * Author: Javier Marrero
 *
 * Created on December 5, 2022, 7:08 PM
 */

#ifndef STACK_H
#define STACK_H

// API
#include <Axf/Core/Object.h>

namespace axf
{
namespace collections
{

/**
 * The <code>Stack</code> interface defines an abstract stack: an ordered
 * sequence of objects that supports insertion and extraction in a LIFO fashion.
 * LIFO means "last-in-first-out": the result of any extraction operation will
 * be the last inserted item.
 * <p>
 * Stacks may be implemented using linked lists or arrays. However, they must
 * guarantee O(1) insertion and extraction. Nor stacks nor queues are designed
 * to be iterated.
 * <p>
 * 
 *
 * @return
 */
template <typename E>
class Stack : virtual public core::Object
{
    AXF_CLASS_TYPE(axf::collections::Stack<E>,
               AXF_TYPE(axf::core::Object))
public:

    /**
     * Pushes an object to the stack. This operation is constant time.
     *
     * @param element
     * @return
     */
    virtual bool push(const E& element) = 0;

    /**
     * Peeks the last value inserted to this stack.
     *
     * @return
     */
    virtual E& peek() const = 0;

    /**
     * Retrieves and removes the last inserted element of this stack. This
     * operation is constant time.
     *
     * @return
     */
    virtual E pop() = 0;
} ;

}
}

#endif /* STACK_H */

