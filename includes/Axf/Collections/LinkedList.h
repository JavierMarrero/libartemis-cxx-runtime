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
 * File:   LinkedList.h
 * Author: Javier Marrero
 *
 * Created on December 4, 2022, 12:45 PM
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// API
#include <Axf/Collections/List.h>
#include <Axf/Collections/DefaultAllocator.h>
#include <Axf/Collections/Iterator.h>
#include <Axf/Core/IndexOutOfBoundsException.h>
#include <Axf/Core/Memory.h>
namespace axf
{
namespace collections
{

namespace
{

/**
 * A node within a linked list.
 */
template <typename E>
struct Node : public axf::core::ReferenceCounted
{
    // Next and previous pointers
    E           m_data;
    Node<E>*    m_next;
    Node<E>*    m_previous;

    /**
     * Constructs a new node
     *
     * @param data
     */
    Node(const E& data) : m_data(data), m_next(NULL), m_previous(NULL) { }

    /**
     * Destructs a node
     */
    ~Node()
    {
        m_next = NULL;
        m_previous = NULL;
    }
} ;

template <typename E>
class LinkedListIterator : public axf::collections::Iterator<E>
{
public:

    LinkedListIterator() : m_current(static_cast<Node<E>*> (Iterator<E>::getBadPointer())) { }

    LinkedListIterator(Node<E>* node) : m_current(node) { }

    virtual E& current()
    {
        return m_current->m_data;
    }

    virtual bool equals(const core::Object& object) const
    {
        const LinkedListIterator<E>& it = static_cast<const LinkedListIterator<E>& > (object);

        return m_current == it.m_current;
    }

    virtual E& next()
    {
        // Store the result
        E& result = m_current->m_data;

        if (m_current->m_next == NULL)
            m_current = static_cast<Node<E>*> (Iterator<E>::getBadPointer());
        else
            m_current = m_current->m_next;

        return result;
    }

private:

    Node<E>* m_current;
} ;

}

/**
 * In computer science, a <i>doubly-linked list</i> is a linked data structure
 * that consists of a set of sequentially linked records called nodes. Each node
 * contains a link to the previous node and the next node in the list.
 * <p>
 * The two node linked list allows for sequential traversing of the list in
 * both directions.
 * 
 * @author J. Marrero
 */
template <typename E, class allocator = axf::collections::DefaultAllocator<Node<E> > >
class LinkedList : public List<E>
{
    AXF_CLASS_TYPE(AXF_TEMPLATE_CLASS(axf::collections::LinkedList<E, allocator>),
                   AXF_TYPE(axf::collections::List<E>))

    // Friend
    template <typename>
    friend class LinkedListIterator;

public:

    /**
     * Constructs a new <code>LinkedList</code> object.
     */
    LinkedList() : m_head(NULL), m_size(0), m_tail(NULL) { }

    /**
     * Destroys the linked list, releasing all allocated memory.
     */
    virtual ~LinkedList()
    {
        while (m_head != NULL)
        {
            Node<E>* deletable = m_head;
            m_head = m_head->m_next;

            m_allocator.deleteObject(deletable);
        }
    }

    /**
     * Adds the element to the end of this linked list.
     *
     * @param element
     * @return
     */
    virtual bool add(const E& element)
    {
        // Create the node
        Node<E>* node = allocateNode(element);
        if (node)
        {
            // Add the node to the end
            insertEnd(node);

            // Increment the size
            ++m_size;
        }
        return node != NULL;
    }

    /**
     * Adds the element at the specified index on the list.
     *
     * @see axf::collections::List
     *
     * @param index
     * @param data
     * @return
     */
    virtual bool add(std::size_t index, const E& data)
    {
        checkIndexOutOfBounds(index);

        // Node pointer
        Node<E>* node = allocateNode(data);
        if (node)
        {
            Node<E>* current = m_head;

            size_t i = 0;
            while (i++ < index)
            {
                // Iterate
                current = current->m_next;
            }

            insertAfter(current, node);
        }

        // Augment the size
        m_size++;
        return node != NULL;
    }

    /**
     * @see axf::collections::Collection::begin
     */
    virtual iterator_ref<E> begin()
    {
        return new LinkedListIterator<E>(m_head);
    }

    /**
     * @see axf::collections::Collection::end
     */
    virtual iterator_ref<E> end()
    {
        return new LinkedListIterator<E>();
    }

    virtual const E& get(std::size_t index) const
    {
        return walk(index)->m_data;
    }

    virtual E& get(std::size_t index)
    {
        return walk(index)->m_data;
    }

    /**
     * @see axf::collections::Collection::isEmpty
     *
     * @return
     */
    virtual bool isEmpty() const
    {
        return m_size == 0;
    }

    /**
     * @see axf::collections::Collection::remove
     *
     * @param element
     * @return
     */
    virtual bool remove(const E& element)
    {
        bool result = false;

        // Find the element
        Node<E>* current = m_head;
        while (current != NULL && !result)
        {
            if (current->m_data == element)
            {
                result = true;

                // Store
                Node<E>* nextCurrent = current->m_next;

                // Remove
                removeNode(current);

                // Iterate
                current = nextCurrent;
            }
            current = current->m_next;
        }

        // Reduce the size
        m_size--;
        return result;
    }

    virtual bool removeAt(std::size_t index)
    {
        if (index >= size())
            return false;

        /* Walk and remove */
        Node<E>* node = walk(index);
        removeNode(node);

        m_size--;
        return true;
    }

    /**
     * @see axf::collections::Collection::size
     *
     * @return
     */
    virtual size_t size() const
    {
        return m_size;
    }

private:

    allocator   m_allocator;    /// The default allocator for linked lists
    Node<E>*    m_head;         /// The head of the list
    size_t      m_size;         /// The size of the list
    Node<E>*    m_tail;         /// The tail of the list

    /**
     * Allocates a new node using the default template provided allocator.
     * 
     * @param data
     * @return
     */
    inline Node<E>* allocateNode(const E& data)
    {
        return m_allocator.newObject(data);
    }

    /**
     * Checks that the provided index is lesser than the size of the collection.
     * If the check fails throws an index out of bounds exception.
     */
    inline void checkIndexOutOfBounds(std::size_t index) const
    {
        if (index >= m_size)
        {
            throw core::IndexOutOfBoundsException("attempted to get an element from the list with an invalid index.", index);
        }
    }

    /**
     * Inserts a node after the specified node.
     *
     * @param node
     * @param newNode
     */
    inline void insertAfter(Node<E>* node, Node<E>* newNode)
    {
        newNode->m_previous = node;
        if (node->m_next == NULL)
        {
            newNode->m_next = NULL;
            m_tail = newNode;
        }
        else
        {
            newNode->m_next = node->m_next;
            node->m_next->m_previous = newNode;
        }
        node->m_next = newNode;
    }

    /**
     * Inserts a node before the specified node
     *
     * @param node
     * @param newNode
     */
    inline void insertBefore(Node<E>* node, Node<E>* newNode)
    {
        newNode->m_next = node;
        if (node->m_previous == NULL)
        {
            newNode->m_previous = NULL;
            m_head = newNode;
        }
        else
        {
            newNode->m_previous = node->m_previous;
            node->m_previous->m_next = newNode;
        }
        node->m_previous = newNode;
    }

    /**
     * Inserts a node at the beginning of a possibly empty list.
     *
     * @param newNode
     */
    inline void insertBeginning(Node<E>* newNode)
    {
        if (m_head == NULL)
        {
            m_head = newNode;
            m_tail = newNode;
        }
        else
        {
            insertBefore(m_head, newNode);
        }
    }

    /**
     * Inserts a node at the end of a possibly empty list.
     *
     * @param newNode
     */
    inline void insertEnd(Node<E>* newNode)
    {
        if (m_tail == NULL)
        {
            insertBeginning(newNode);
        }
        else
        {
            insertAfter(m_tail, newNode);
        }
    }

    inline void removeNode(Node<E>* node)
    {
        if (node->m_previous == NULL)
        {
            // It is the first element
            m_head = node->m_next;
        }
        else
        {
            node->m_previous->m_next = node->m_next;
        }

        if (node->m_next == NULL)
        {
            // It is the last element
            m_tail = node->m_previous;
        }
        else
        {
            node->m_next->m_previous = node->m_previous;
        }

        m_allocator.deleteObject(node);
    }

    /**
     * Walk through the nodes until reaching index.
     * 
     * @param index
     * @return
     */
    inline Node<E>* walk(std::size_t index) const
    {
        checkIndexOutOfBounds(index);

        Node<E>* current = const_cast<Node<E>* > (m_head);

        ///TODO: Optimize and make the algorithm O(n/2) instead of O(n)
        if (index > 0)
        {
            do
            {
                current = current->m_next;
            }
            while (--index > 0);
        }
        return current;
    }

} ;

}
}

#endif /* LINKEDLIST_H */

