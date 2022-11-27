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
 * File:   unique_ptr.h
 * Author: Javier Marrero
 *
 * Created on November 27, 2022, 2:35 AM
 */

#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

// Include the abstract reference
#include "abstract_ref.h"

namespace axf
{
namespace core
{

template <typename T>
class unique_ref : public bits::abstract_ref<T>
{
    
};

}
}

#endif /* UNIQUE_PTR_H */
