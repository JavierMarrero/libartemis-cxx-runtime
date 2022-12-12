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
 * File:   search.h
 * Author: Javier Marrero
 *
 * Created on December 11, 2022, 4:21 PM
 */

#ifndef SEARCH_H
#define SEARCH_H

// API
#include <Axf/Collections/Algorithms/default-predicates.h>

namespace axf
{
namespace algorithms
{

/**
 * Searches for the first occurrence of the sequence of elements [s_first, s_last)
 * in the range [first, last). The provided binary predicate may compare any
 * binary relationship, not just equality.
 *
 * @param first
 * @param last
 * @param begin
 * @param end
 * @param p
 * @return
 */
template <class forward_it1, class forward_it2, class binary_predicate>
forward_it1 search(forward_it1 first, forward_it1 last, forward_it2 begin, forward_it2 end, binary_predicate p)
{
    while (1)
    {
        forward_it1 it = first;
        for (forward_it2 s_it = begin; ; ++it, ++s_it)
        {
            if (s_it == end) return first;
            if (it == last) return last;
            if (!p(*it, *s_it)) break;
        }
        ++first;
    }
}

}
}

#endif /* SEARCH_H */

