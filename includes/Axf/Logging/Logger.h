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
 * File:   Logger.h
 * Author: Javier Marrero
 *
 * Created on December 5, 2022, 8:01 PM
 */

#ifndef LOGGER_H
#define LOGGER_H

// API
#include <Axf/Core/Object.h>
#include <Axf/Core/String.h>

// C++
#include <climits>

namespace axf
{
namespace logging
{

/**
 * The <code>Logger</code> interface represents an object that is able to
 * write logging messages to a determined output.
 * <p>
 * Logging messages are useful in the development and deployment phases of
 * application development. It serves both developers and users. The difference
 * with simple console output lies in two principal attributes:
 * <ul>
 *  <li>level filtering: log messages may be filtered by level</li>
 *  <li>attributes: log messages have attributes (like timestamps)</li>
 * </ul>
 * <p>
 * This interface is the base to the structured logging framework of the
 * <i>Artemis Logging Framework</i>.
 */
class Logger : public core::Object
{

    AXF_CLASS_TYPE(axf::logging::Logger,
                   AXF_TYPE(axf::core::Object))

public:

    /**
     * This enumeration holds all the possible log level values.
     * <p>
     * Notice the log levels must be sequential and their integer value must be
     * suitable for use as an array index.
     */
    typedef enum LogLevel
    {
        ALL = -1,
        TRACE = 0,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        OFF = INT_MAX
    } LogLevel;

protected:

    static const core::string& getLevelString(const LogLevel level);
} ;

}
}

#endif /* LOGGER_H */

