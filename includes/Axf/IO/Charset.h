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
 * File:   Charset.h
 * Author: Javier Marrero
 *
 * Created on December 15, 2022, 9:40 AM
 */

#ifndef CHARSET_H
#define CHARSET_H

// API
#include <Axf/Core/Object.h>

namespace axf
{
namespace io
{
namespace charset
{

/**
 * The <code>Charset</code> class plays an important role in encoding and
 * decoding text from and to different character sets.
 * <p>
 * A character set is the composite number of different characters that are
 * being used and supported by hardware/software. It consists of codes, bit
 * patterns or natural numbers used in some particular character.
 * <p>
 * All word/data processing applications are embedded with one or more character
 * sets. The characters within a character set can be text, numbers or even
 * symbols.
 * <p>
 * This class handles encoding and decoding of characters in different formats.
 *
 * @author J. Marrero
 */
class Charset : public core::Object
{
    AXF_CLASS_TYPE(axf::io::charset::Charset, AXF_TYPE(axf::core::Object))
public:

    /**
     * Returns a character set suitable for encoding or decoding the specified
     * id.
     *
     * @param name
     * @return
     */
    static Charset forName(const char* name);

    /**
     * Default destructor
     */
    ~Charset() { }

private:

    const char* m_identifier;

    Charset(const char* identifier);
} ;

}
}
}

#endif /* CHARSET_H */

