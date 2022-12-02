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
 * File:   Version.h
 * Author: Javier Marrero
 *
 * Created on August 15, 2022, 12:35 AM
 */

#ifndef __SELENIUM_VERSION_H
#define __SELENIUM_VERSION_H

/* Version macros */
#ifndef __ARTEMIS_VERSION_MACROS_SET
#define  __ARTEMIS_VERSION_MACROS_SET 1

#define _ARTEMIS_MAJOR_VERSION     0
#define _ARTEMIS_MINOR_VERSION     1
#define _ARTEMIS_REVISION_VERSION  0

#endif

namespace axf
{
namespace api
{

/**
 * This enumerated type defines the different version fields for the library.
 * It relies on the version macros, this is just a 'typesafe' way to access
 * these fields. They must be used throughout the code instead of the macros.
 */
enum Version
{
    MAJOR = _ARTEMIS_MAJOR_VERSION,
    MINOR = _ARTEMIS_MINOR_VERSION,
    REVISION = _ARTEMIS_REVISION_VERSION
} ;
typedef enum Version Version;

}
}

#endif /* VERSION_H */

