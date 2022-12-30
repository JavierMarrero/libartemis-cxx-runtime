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
 * File:   Axf.h - Artemis Extended Framework main header file
 * Author: Javier Marrero
 *
 * Created on November 27, 2022, 1:11 AM
 */

#ifndef AXF_H
#define AXF_H

// C++
#include <cstddef>

// API
#include <Axf/API/Compiler.h>
#include <Axf/API/Platform.h>
#include <Axf/API/Version.h>

#include <Axf/Collections/Algorithms.h>
#include <Axf/Collections/Allocator.h>
#include <Axf/Collections/ArrayList.h>
#include <Axf/Collections/Collection.h>
#include <Axf/Collections/ConcurrentModificationException.h>
#include <Axf/Collections/DefaultAllocator.h>
#include <Axf/Collections/HashMap.h>
#include <Axf/Collections/HashSet.h>
#include <Axf/Collections/Iterable.h>
#include <Axf/Collections/Iterator.h>
#include <Axf/Collections/LinkedList.h>
#include <Axf/Collections/List.h>
#include <Axf/Collections/Map.h>
#include <Axf/Collections/NoSuchElementException.h>
#include <Axf/Collections/Queue.h>
#include <Axf/Collections/Set.h>
#include <Axf/Collections/Stack.h>

#include <Axf/Core/Lang-C++/keywords.h>
#include <Axf/Core/Lang-C++/move.h>
#include <Axf/Core/Lang-C++/pointers.h>
#include <Axf/Core/Lang-C++/reference-wrapper.h>
#include <Axf/Core/Lang-C++/traits.h>
#include <Axf/Core/Lang-C++/varargs.h>

#include <Axf/Core/Array.h>
#include <Axf/Core/Character.h>
#include <Axf/Core/Class.h>
#include <Axf/Core/ClassCastException.h>
#include <Axf/Core/Comparator.h>
#include <Axf/Core/Exception.h>
#include <Axf/Core/IllegalArgumentException.h>
#include <Axf/Core/IllegalOperationException.h>
#include <Axf/Core/IllegalStateException.h>
#include <Axf/Core/IndexOutOfBoundsException.h>
#include <Axf/Core/Memory.h>
#include <Axf/Core/NullPointerException.h>
#include <Axf/Core/Number.h>
#include <Axf/Core/Object.h>
#include <Axf/Core/OutOfMemoryError.h>
#include <Axf/Core/ReferenceCounted.h>
#include <Axf/Core/Runtime.h>
#include <Axf/Core/String.h>

#include <Axf/IO/BufferedOutputStream.h>
#include <Axf/IO/Charset.h>
#include <Axf/IO/Closeable.h>
#include <Axf/IO/File.h>
#include <Axf/IO/FileNotFoundException.h>
#include <Axf/IO/FileOutputStream.h>
#include <Axf/IO/IOException.h>
#include <Axf/IO/IOStream.h>
#include <Axf/IO/OutputStream.h>

#include <Axf/Logging/Logger.h>

#include <Axf/Math/BasicMath.h>

#include <Axf/Text/Regex/CharacterMatcher.h>
#include <Axf/Text/Regex/Compiler.h>
#include <Axf/Text/Regex/EpsilonMatcher.h>
#include <Axf/Text/Regex/Matcher.h>
#include <Axf/Text/Regex/Pattern.h>
#include <Axf/Text/Regex/PatternSyntaxException.h>
#include <Axf/Text/Regex/Regex.h>
#include <Axf/Text/Regex/State.h>
#include <Axf/Text/Regex/Tokens.h>

#include <Axf/Threading/Thread.h>

#include <Axf/Utils/Pair.h>

#endif /* AXF_H */
