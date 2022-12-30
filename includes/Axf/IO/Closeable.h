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
 * File:   Closeable.h
 * Author: Javier Marrero
 *
 * Created on December 28, 2022, 12:41 PM
 */

#ifndef CLOSEABLE_H
#define CLOSEABLE_H

// API
#include <Axf/Core/Object.h>

namespace axf
{
namespace io
{

/**
 * The <code>Closeable</code> interface is an interface consisting of the
 * <code>close</code> method, which releases all acquired I/O resources.
 * <p>
 * The <code>Closeable</code> interface provides safety on resource handling,
 * since when objects implementing this interface are destroyed their associated
 * resources are automatically released. This frees the programmer from manually
 * managing the resources.
 * <p>
 * <b>Note</b>: Implementations of the close method on subjacent classes must
 * be <b><i>idempotent</i></b>, which means that successive applications of
 * the close function must not have any secondary effects. This is mandatory
 * for correct functioning of the closeable classes.
 *
 * @author J. Marrero
 */
class Closeable : virtual public core::Object
{

    AXF_CLASS_TYPE(axf::io::Closeable, AXF_TYPE(axf::core::Object))

public:

    /**
     * Destroys a closeable object, releasing to the system all the allocated
     * resources.
     */
    virtual ~Closeable()
    {
        close();
    }

    /**
     * Releases to the system all the associated resources to this object.
     * <p>
     * Implementors of this method must make the implementation idempotent,
     * such as when reapplying the method no secondary effects must be observable.
     */
    virtual void close() { };
} ;

}
}

#endif /* CLOSEABLE_H */

