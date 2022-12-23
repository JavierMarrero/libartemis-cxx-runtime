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
 * File:   move.h
 * Author: Javier Marrero
 *
 * Created on December 18, 2022, 8:29 PM
 */

#ifndef MOVE_H
#define MOVE_H

// API
#include <Axf/API/Compiler.h>

namespace axf
{
namespace core
{

#if !ARTEMIS_CXX11_SUPPORTED

/**
 * This structure emulates rvalue references in C++98 compilers.
 */
template <class T>
struct rvalue : public T
{
private:

    rvalue();
    ~rvalue() throw();
    rvalue(const rvalue&);

    void operator=(rvalue const&);
} ;

#endif

}
}

#endif /* MOVE_H */

