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
 * File:   PatternSyntaxException.h
 * Author: Javier Marrero
 *
 * Created on December 24, 2022, 12:14 PM
 */

#ifndef PATTERNSYNTAXEXCEPTION_H
#define PATTERNSYNTAXEXCEPTION_H

// API
#include <Axf/Core/Exception.h>

namespace axf
{
namespace text
{
namespace regex
{

/**
 * This class of exceptions describes problems when parsing exceptions.
 *
 * @author J. Marrero
 */
class PatternSyntaxException : public core::Exception
{
    AXF_EXCEPTION_TYPE(axf::text::regex::PatternSyntaxException,
                       axf::core::Exception)

public:

    PatternSyntaxException(const char* message);
    ~PatternSyntaxException();

} ;

}
}
}

#endif /* PATTERNSYNTAXEXCEPTION_H */

