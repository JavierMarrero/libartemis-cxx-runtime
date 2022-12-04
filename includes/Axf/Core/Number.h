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
 * File:   Number.h
 * Author: Javier Marrero
 *
 * Created on December 2, 2022, 2:21 PM
 */

#ifndef NUMBER_H
#define NUMBER_H

// API
#include <Axf/Core/Object.h>

namespace axf
{
namespace core
{

/**
 * The <code>Number</code> class is the base for all numeric types wrapper
 * objects.
 * <p>
 */
class Number : public Object
{
    AXF_OBJECT(axf::core::Number, AXF_TYPE(axf::core::Object))

public:

} ;

}
}

#endif /* NUMBER_H */

