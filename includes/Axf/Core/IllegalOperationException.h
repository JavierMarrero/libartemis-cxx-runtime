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
 * File:   IllegalOperationException.h
 * Author: Javier Marrero
 *
 * Created on December 5, 2022, 11:29 PM
 */

#ifndef ILLEGALOPERATIONEXCEPTION_H
#define ILLEGALOPERATIONEXCEPTION_H

// API
#include <Axf/Core/Exception.h>

namespace axf
{
namespace core
{

/**
 * Signals the invocation of an illegal operation, such as when one invoke a
 * method that is not properly defined for a particular class. For example,
 * when one attempts to effect an operation on a data structure and that
 * operation is not available for the particular data structure.
 * <p>
 * The difference with <code>IllegalStateException</code> is that the former
 * indicates illegal state, while this indicates illegal invocation. The state
 * may be correct but the act of invoking is what is wrong.
 * <p>
 * This class of runtime errors is strange, and it must be avoided by carefully
 * designing interface as to not to need this class at all.
 * <p>
 * However, for the faint of heart, here it is... ¬_¬
 *
 * @author J. Marrero
 */
class IllegalOperationException : public Exception
{
    AXF_EXCEPTION_TYPE(axf::core::IllegalOperationException,
                       axf::core::Exception)
public:

    IllegalOperationException(const char* message, const char* method);
    ~IllegalOperationException();

    /**
     * Retrieves the method name of the illegal operation that was invoked.
     *
     * @return
     */
    inline const char* getMethodName() const
    {
        return m_method;
    }

private:

    char m_method[256];

} ;

}
}

#endif /* ILLEGALOPERATIONEXCEPTION_H */

