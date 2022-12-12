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
 * File:   Exception.cpp
 * Author: Javier Marrero
 * 
 * Created on November 27, 2022, 4:48 PM
 */

#include <Axf/Core/Exception.h>
#include <Axf/Core/IllegalStateException.h>

// C
#include <cstring>

// OBSCURE DECLARATIONS
// PLEASE, DO NOT MESS WITH THIS STUFF
// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

typedef struct eh_frame eh_frame_t;

void pushExceptionHandlingFrame(const axf::core::Exception* exception);
eh_frame_t* popExceptionHandlingFrame();

// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
// THOSE WERE OBSCURE FUNCTIONS
// IF YOU MESS WITH THOSE YOU WILL BE DAMNED FOREVER
// AND YOU WILL NEVER FIND BOYFRIEND OR GIRLFRIEND
// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

using namespace axf;
using namespace axf::core;
using namespace axf::core::bits;

ExceptionTypeDescriptor::ExceptionTypeDescriptor(const char* className, const ExceptionTypeDescriptor* super)
:
m_className(className),
m_super(super)
{
}

ExceptionTypeDescriptor::~ExceptionTypeDescriptor()
{
}

bool ExceptionTypeDescriptor::isInstanceOf(const ExceptionTypeDescriptor& exceptionType) const
{
    return this == &exceptionType;
}

bool ExceptionTypeDescriptor::isKindOf(const ExceptionTypeDescriptor& exceptionType) const
{
    const ExceptionTypeDescriptor* current = this;
    while (current != NULL)
    {
        if (current == &exceptionType)
        {
            return true;
        }
        current = current->m_super;
    }
    return false;
}

const ExceptionTypeDescriptor& ExceptionTypeDescriptor::super() const
{
    if (m_super == NULL)
        throw IllegalStateException("attempted to retrieve the super-type of a base class!");
    return *m_super;
}

const ExceptionTypeDescriptor& Exception::getCompileTimeClass()
{
    static ExceptionTypeDescriptor descriptor("axf::core::Exception", NULL);

    // Return a reference to the newly allocated constructor
    return descriptor;
}

Exception::Exception(const char* message)
{
    std::strncpy(m_message, message, 1024);

    // Push an new exception handling frame
    pushExceptionHandlingFrame(this);
}

Exception::~Exception()
{
    // Pop the old exception handling frame
    popExceptionHandlingFrame();
}

const bits::ExceptionTypeDescriptor& Exception::getClass() const
{
    return getCompileTimeClass();
}

const char* Exception::getClassName() const
{
    return "axf::core::Exception";
}

// ************************************************************************** //
//
// This is private exception handling code
// Please, do not mess up with this
// This warning sign should not be ignored... else, curse you ¬_¬
//

// Some cool API includes
#include <Axf/API/Compiler.h>

// Plain old C... the good stuff
#include <cstdlib>
#include <cassert>

// Thread local storage the old way
#ifdef ARTEMIS_COMPILER_GCC_COMPATIBLE
#    define _tls __thread
#else
#    define _tls __declspec(thread)
#endif

#ifndef _tls
#    error "We needed TLS to be defined, bitch..."
#endif

/**
 * The descriptor for the exception handling frames.
 */
typedef struct eh_frame
{
    const Exception* m_current;
    struct eh_frame* m_previous;
} eh_frame_t;

/**
 * This thread local variable is a pointer to a stack of exceptions. Every time
 * an exception is raised, it is added to the handling frame of the thread.
 * Then, if an exception goes un-handled, this stack is unwound, and a descriptive
 * error message is displayed.
 */
_tls eh_frame_t* eh_frame_stack = NULL;

static eh_frame_t* createEhFrame(const Exception* ex)
{
    // Allocate memory (in the C heap, the C++ heap may be corrupted at this point)
    eh_frame_t* frame = static_cast<eh_frame_t*> (std::calloc(1, sizeof (eh_frame_t)));
    assert(frame != NULL && "wow, even this failed this time...");

    // Fill in the data
    frame->m_current = ex;
    frame->m_previous = eh_frame_stack;

    // Return
    return frame;
}

void deleteEhFrame(eh_frame_t* frame)
{
    std::free(frame);
}

void pushExceptionHandlingFrame(const axf::core::Exception* exception)
{
    eh_frame_stack = createEhFrame(exception);
}

eh_frame_t* popExceptionHandlingFrame()
{
    assert(eh_frame_stack != NULL && "why are we popping when we didn't push?????");

    eh_frame_t* frame = eh_frame_stack;
    eh_frame_stack = frame->m_previous;

    return frame;
}

const Exception* peekLastException()
{
    assert(eh_frame_stack != NULL && "why!!!!!! why on earth???");

    return eh_frame_stack->m_current;
}

const eh_frame_t* currentExceptionFrame()
{
    return eh_frame_stack;
}

const eh_frame_t* unwindExceptionFrames(const eh_frame_t* current)
{
    return current->m_previous;
}

const Exception* getException(const eh_frame_t* current)
{
    return current->m_current;
}
