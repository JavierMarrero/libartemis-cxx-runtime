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
 * File:   rtti_test.cpp
 * Author: Javier Marrero
 *
 * Created on December 2, 2022, 7:24 PM
 */

#include <stdlib.h>
#include <iostream>

#include <Axf.h>

using namespace axf;
using namespace axf::core;

class AClass : virtual public core::Object
{
    AXF_CLASS_TYPE(AClass, AXF_TYPE(axf::core::Object))
public:

} ;

class AInterface : virtual public core::Object
{
    AXF_CLASS_TYPE(AInterface, AXF_TYPE(axf::core::Object))
public:


} ;

class AMultipleInherited : public AClass, public AInterface
{
    AXF_CLASS_TYPE(AMultipleInherited, AXF_TYPE(AClass), AXF_TYPE(AInterface))

public:


} ;

class UnrelatedType : public Object
{
    AXF_CLASS_TYPE(UnrelatedType, AXF_TYPE(Object))
public:
} ;

int main(int argc, char** argv)
{
    try
    {

        AClass aClass;
        AMultipleInherited aMultipleInherited;

        // Name qualification
        std::cout << "Object class qualified name: " << Object::getCompileTimeClass().getName() << " | unqualified: " << Object::getCompileTimeClass().getSimpleName() << std::endl;

        // Test AClass
        std::cout << "AClass: " << AClass::getCompileTimeClass().getName() << std::endl;
        std::cout << "AClass polymorphic type: " << aClass.getClass<AClass>().getName() << std::endl;
        std::cout << "AClass super type: " <<
                reflection::asClass<core::Object>(aClass.getClass<AClass>().getPrimarySuperType(), AXF_TYPENAME(axf::core::Object)).getName() << std::endl;

        // Test multiple inheritance
        std::cout << "AMultipleInherited: " << AMultipleInherited::getCompileTimeClass().getName() << std::endl;
        std::cout << "AMultipleInherited polymorphic type: " << aMultipleInherited.getClass<AMultipleInherited>().getName() << std::endl;
        std::cout << "AMultipleInherited super type: " <<
                reflection::asClass<AClass>(aMultipleInherited.getClass<AMultipleInherited>().getPrimarySuperType(),
                                            AXF_TYPENAME(AClass)).getName() << std::endl;

        // Test class retrieval
        std::cout << "Retrieve a Object from a 2nd level derivative: " <<
                reflection::getClass(aMultipleInherited).getSuperClass(AXF_TYPENAME(axf::core::Object)).getName() << std::endl;
        std::cout << "is AMultipleInherited a AClass? " << reflection::getClass(aMultipleInherited).isKindOf(AClass::getCompileTimeClass()) << std::endl;
        std::cout << "is AMultipleInherited an Object? " << reflection::getClass(aMultipleInherited).isKindOf(Object::getCompileTimeClass()) << std::endl;
        std::cout << "is AMultipleInherited an AInterface? " << reflection::getClass(aMultipleInherited).isKindOf(AInterface::getCompileTimeClass()) << std::endl;
        std::cout << "is AMultipleInherited an UnrelatedType? " << reflection::getClass(aMultipleInherited).isKindOf(UnrelatedType::getCompileTimeClass()) << std::endl;
        std::cout << "is aMultipleInherited exactly a AMultipleInherited? " << reflection::getClass(aMultipleInherited).isInstanceOf(AMultipleInherited::getCompileTimeClass()) << std::endl;
        std::cout << "is aMultipleInherited exactly an Object? " << reflection::getClass(aMultipleInherited).isInstanceOf(Object::getCompileTimeClass()) << std::endl;

        // Test dynamic_cast
        Object& object = reflection::runtime_cast<Object>(aMultipleInherited);
        std::cout << "object hash code: " << object.hashCode() << " | size: " << object.getClass<AMultipleInherited>().sizeOf << std::endl;

        // Object sizes
        std::cout << "\nObject sizes:" << std::endl;
        std::cout << "Object: " << Object::getCompileTimeClass().sizeOf << std::endl;
        std::cout << "AClass: " << AClass::getCompileTimeClass().sizeOf << std::endl;
        std::cout << "AMultipleInherited: " << AMultipleInherited::getCompileTimeClass().sizeOf << std::endl;

        // Check for exception safety
        try
        {
            const Class<UnrelatedType>& wrongType = reflection::asClass<UnrelatedType>(aMultipleInherited.getClass<AMultipleInherited>().getPrimarySuperType(),
                                                                                       AXF_TYPENAME(UnrelatedType));
            std::cout << "why?? " << wrongType.getName() << std::endl;
        }
        catch (ClassCastException& ex)
        {
            std::cerr << "\n" << ex.getClassName() << ": " << ex.getMessage() << std::endl;
        }

        std::cout << "aMultipleInherited: " << aMultipleInherited.toString() << std::endl;
    }
    catch (Exception& ex)
    {
        std::cerr << "\n" << ex.getClassName() << ": " << ex.getMessage() << std::endl;
    }

    std::cout << "finished..." << std::endl;
    std::getchar();
    return (EXIT_SUCCESS);
}

