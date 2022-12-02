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
 * File:   smart_references.cpp
 * Author: Javier Marrero
 *
 * Created on November 28, 2022, 5:02 PM
 */

#include <stdlib.h>
#include <iostream>
#include <cstdio>

#include <Axf.h>

class Dummy
{
public:

    Dummy()
    {
        std::cout << "creating a new dummy at " << this << std::endl;
    }

    ~Dummy()
    {
        std::cout << "deleting a dummy at " << this << std::endl;
    }
} ;

class DummyIntrusive : public axf::core::ReferenceCounted
{
public:

    DummyIntrusive()
    {
        std::cout << "creating new intrusive reference counted dummy at " << this << std::endl;
    }

    virtual ~DummyIntrusive()
    {
        std::cout << "deleting intrusive dummy at " << this << std::endl;
    }

} ;

void test_scoped_ptr()
{
    axf::core::scoped_ref<Dummy> dummy = new Dummy();
    axf::core::scoped_ref<Dummy> dummy2 = new Dummy();

    if (dummy == dummy2)
    {
        std::cout << "pointers are equals" << std::endl;
    }
    else
    {
        std::cout << "pointers are not equals" << std::endl;
    }

    // Dummy2 must be deleted
    std::cout << "transferring ownership from object 2 to object 1." << std::endl;
    dummy = dummy2;
}

void test_strong_ptr()
{
    // Non intrusive variant
    {
        axf::core::strong_ref<Dummy> dummy = new Dummy();
        std::cout << "references: " << dummy.users() << std::endl;

        axf::core::strong_ref<Dummy> dummy2 = new Dummy();
        std::cout << "references to 2nd object: " << dummy.users() << std::endl;

        // Create another in another scope
        {
            axf::core::strong_ref<Dummy> dummy3 = dummy;
            std::cout << "references on sharing: " << dummy3.users() << std::endl;
        }

        std::cout << "references after releasing: " << dummy.users() << std::endl;
        dummy2 = dummy;

        std::cout << "references after assignment: " << dummy.users() << std::endl;
        std::cout << "dropping all references..." << std::endl;
    }
    // Intrusive variant
    {
        axf::core::strong_ref<DummyIntrusive> dummy = new DummyIntrusive();
        std::cout << "original references: " << dummy.users() << std::endl;
        {
            axf::core::strong_ref<DummyIntrusive> dummy2 = dummy;
            std::cout << "references to intrusive dummy: " << dummy2.users() << std::endl;
        }

        std::cout << "dropping all references..." << std::endl;
    }
}

void test_weak_ptr()
{
    // Non-intrusive variant
    axf::core::strong_ref<Dummy> dumdum = new Dummy();
    {
        axf::core::weak_ref<Dummy> dummy = dumdum;
        std::cout << "references: " << dummy.users() << std::endl;

        {
            axf::core::weak_ref<Dummy> dummy2 = dummy;
            std::cout << "references after reassignment: " << dummy2.users() << std::endl;
            std::cout << "users: (strong " << dumdum.users() << ") (weak " << dummy.users() << ")." << std::endl;
        }
        std::cout << "dropping all references... now " << dummy.users() << std::endl;
    }
    // Intrusive variant
    axf::core::strong_ref<DummyIntrusive> sdummy = new DummyIntrusive();
    {
        axf::core::weak_ref<DummyIntrusive> weak1 = sdummy;
        std::cout << "strong: " << sdummy.users() << ", weak: " << weak1.users() << std::endl;

        {
            axf::core::weak_ref<DummyIntrusive> weak2 = weak1;
            std::cout << "after sharing: " << weak2.users() << std::endl;
        }

        std::cout << "(2) strong: " << sdummy.users() << ", weak: " << weak1.users() << std::endl;
    }
    std::cout << "dropping all references" << std::endl;
}

int main(int argc, char** argv)
{
    std::printf("\ntesting unique pointers...\n");
    test_scoped_ptr();

    std::printf("\ntesting shared strong pointers...\n");
    test_strong_ptr();

    std::printf("\ntesting shared weak pointers...\n");
    test_weak_ptr();

    std::getchar();
    return (EXIT_SUCCESS);
}

