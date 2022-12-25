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
 * File:   Queue.h
 * Author: Javier Marrero
 *
 * Created on December 5, 2022, 1:53 AM
 */

#ifndef QUEUE_H
#define QUEUE_H

// API
#include <Axf/Core/Object.h>

namespace axf
{
namespace collections
{

/**
 * Defines a queue, an abstract data type presenting first-come-first-served
 * behavior.
 * <p>
 * Queues are abstract data types, and as such, nothing is declared to be a
 * concrete instance of a queue. It is characterized for being an ordered
 * sequence of elements into which the insertion operation (push) is performed
 * on opposite ends to the extraction operation (pop).
 * <p>
 * The <code>Queue</code> interface is conceptually extended in the <code>
 * Deque</code> interface, that abstract the concept of doubly-ended queues.
 * <p>
 * Concrete queues may be implemented using linked lists or circular queues
 * using arrays.
 *
 * @author J. Marrero
 */
template <typename E>
class Queue : virtual public axf::core::Object
{
    AXF_CLASS_TYPE(axf::collections::Queue<E>,
               AXF_TYPE(axf::core::Object))

public:

    /**
     * Offers an element to the queue. In other words, inserts an element into
     * the queue.
     *
     * @param element
     * @return
     */
    virtual bool offer(const E& element) = 0;

    /**
     * Peeks the element on top of this queue.
     *
     * @return
     */
    virtual E& peek() const = 0;

    /**
     * Retrieves and extracts the corresponding element that goes next in the
     * queue.
     *
     * @param element
     * @return
     */
    virtual E poll() = 0;
} ;

}
}

#endif /* QUEUE_H */

