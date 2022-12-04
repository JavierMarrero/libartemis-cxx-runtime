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
 * File:   Class.h
 * Author: Javier Marrero
 *
 * Created on December 2, 2022, 1:41 PM
 */

#ifndef AXF_CLASS_H
#define AXF_CLASS_H

// API
#include <Axf/Core/ReferenceCounted.h>
#include <Axf/Core/ClassCastException.h>
#include <Axf/Core/IllegalStateException.h>

// C++
#include <exception>
#include <stdexcept>
#include <vector>

// C
#include <cstddef>
#include <cstdarg>
#include <cstdio>

namespace axf
{
namespace core
{

// Forward declaration of the object class
class Object;

namespace bits
{

/**
 * Converts a type to a <code>const char*</code> string directly usable as
 * type name.
 * <p>
 * Notice this does not works with variables, rather pass the type of the
 * variable.
 */
#define AXF_TYPENAME(...) #__VA_ARGS__

/**
 * Tag struct for Class class. It allows to do some basic type erasure while
 * keeping some basic data available.
 * <p>
 * @author J. Marrero
 */
struct Type : public ReferenceCounted
{
protected:

    const char* m_className;    /// The class name that identifies this type
    int m_hash;

    /** Creates a new type object */
    Type(const char* className) : m_className(className), m_hash(encodeTypeName(className)) { }

public:

    /**
     * Returns a 32-bit hash that uniquely identifies this type. All equality
     * comparison between types is performed using the type name hash codes
     * for speed.
     * 
     * @param str
     * @return
     */
    static int encodeTypeName(const char* str);

    /**
     * Return true if two types are equals. Two types are equals if their hash
     * code matches exactly.
     * <p>
     * The equality relationship is symmetric, reflexive and transitive.
     * 
     * @param type
     * @return
     */
    inline bool equals(const Type& type) const
    {
        return m_hash == type.m_hash;
    }

    /**
     * Returns true if a type name matches this type's. Comparison is performed
     * using the hash codes for performance reasons.
     * 
     * @param className
     * @return
     */
    inline bool equals(const char* className) const
    {
        return m_hash == encodeTypeName(className);
    }

    /**
     * Returns the name of this type. The name of this type is provided at
     * creation time. It must return a fully qualified name, though this is
     * not guaranteed for classes external to the library.
     * <p>
     * Internal classes all are declared with their fully qualified names as
     * arguments to the <code>AXF_OBJECT</code> macro, so this is safe to
     * use within the library.
     * <p>
     * For external, untrusted code, there are no guarantees. Guarantees are
     * impossible to enforce in this context since all the reflection within
     * the library is achieved using macro trickery.
     * 
     * @return
     */
    inline const char* getName() const
    {
        return m_className;
    }

    /**
     * Returns the unqualified name of this class. The unqualified name of a
     * class is the class name without any name-space specifiers, <i>i.e</i>
     * for the class <code>axf::core::Class</code> the unqualified name would
     * be <code>Class</code>
     * <p>
     * The returned string is the same pointer to the qualified class name, but
     * offset accordingly.
     * 
     * @return
     */
    const char* getSimpleName() const;

    /**
     * Returns the type hash associated to this type. The hashing is performed
     * via the class name.
     * 
     * @return
     */
    inline const int getTypeHash() const
    {
        return m_hash;
    }

} ;

}

/**
 * The <code>Class</code> class represents the runtime type information of a
 * given object within the framework.
 * <p>
 * Class descriptors allows some kind of reflexive programming, since they
 * permit reasoning over data types at runtime. This, with the help of type
 * traits (computed at compile type) allows a fairly good level of reasoning
 * over types and the structure of code.
 * <p>
 * Every object descendant of <code>axf::core::Object</code> and that
 * incorporates the <code>AXF_OBJECT<code> has an associated class descriptor.
 *
 * @author J. Marrero
 */
template <typename T>
class Class : public bits::Type
{
public:

    typedef T   rawType;            /// The raw type of this class
    typedef T*  rawPointerType;     /// The raw pointer type of this class

    static const size_t sizeOf = sizeof (T); /// The size of the underlying type

    /**
     * Returns the <code>Class<T>/code> object that represents the type T
     * in the type system.
     * <p>
     * 
     * @return
     */
    static const Class<T>& classObject()
    {
        return T::getCompileTimeClass();
    }

    /**
     * Creates a new instance of the class object. The passed super list of
     * objects must be null terminated, else infinite loop will occur. This
     * class is normally not directly built by users, so this must be fairly
     * safe.
     */
    Class(const char* className, const bits::Type* super, ...)
    :
    bits::Type(className)
    {
        // Push back the first super-type
        m_superTypes.push_back(super);

        // Now push the rest of the types until a NULL is encountered.
        std::va_list va;
        va_start(va, super);

        bits::Type* currentType = NULL;
        while ((currentType = va_arg(va, bits::Type*)) != NULL)
        {
            // It must not contain repeated types
            if (isDirectSuperClass(*currentType) == false)
            {
                m_superTypes.push_back(currentType);
            }
        }

        va_end(va);
    }

    /**
     * Default destructor of a class object.
     */
    ~Class() { }

    /**
     * Gets a reference to a class that is a direct super type of this class
     * searching by name.
     * 
     * @param className
     * @return
     */
    inline const bits::Type& getDirectSuperTypeByName(const char* className) const
    {
        for (int i = 0, size = m_superTypes.size(); i < size; ++i)
        {
            if (m_superTypes.at(i)->equals(className))
            {
                return *m_superTypes.at(i);
            }
        }
        return NULL;
    }

    /**
     * The primary super type of a class
     *
     * @return
     */
    inline const bits::Type& getPrimarySuperType() const
    {
        if (m_superTypes.empty())
            throw IllegalStateException("attempted to get superclass of a basic object.");
        return *(m_superTypes.front());
    }

    /**
     * Returns the superclass of this class with the specified fully qualified
     * name. The class name passed as argument must be <b>fully qualified</b>
     * (<i>i.e</i> as in <code>axf::core::Object</code> for this method
     * to work, else assume not to find any super class of the given name)
     * <p>
     * This method walks the inheritance graph recursively. If no super class
     * with the given name is found, an <code>IllegalStateException</code>
     * exception will be thrown.
     * 
     * @param className
     * @return
     */
    inline const bits::Type& getSuperClass(const char* className) const
    {
        const bits::Type* currentWalker = NULL;
        for (unsigned i = 0; i < m_superTypes.size() && currentWalker == NULL; ++i)
        {
            // Recursively walk the graph
            currentWalker = walkInheritanceGraph(className, (const Class<T>*) m_superTypes.at(i));
        }

        // If no given super-type is found
        if (currentWalker == NULL)
        {
            char exceptionMessage[1024] = {0};
            std::sprintf(exceptionMessage, "invalid super-type look-out, '%s' is not a valid '%s' subtype",
                         m_className,
                         className);

            throw IllegalStateException(exceptionMessage);
        }

        // Returns a reference to the current walker
        return *currentWalker;
    }

    /**
     * Checks if the provided type is a superclass of this type. A type <code>A</code>
     * is a superclass of type <code>B</code> if and only if there is a covariance
     * relationship between A and B.
     * <p>
     * This method only search within its direct ancestors.
     * 
     * @param type
     * @return
     */
    inline bool isDirectSuperClass(const bits::Type& type)
    {
        for (unsigned i = 0, size = m_superTypes.size(); i < size; ++i)
        {
            if (m_superTypes.at(i) == &type)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * Returns true if a given instance of a class. The object must be an
     * instance of the class and of that class only.
     * <p>
     * For a behavior similar to Java´s <code>instanceof</code> operator,
     * use the <code>isKindOf</code> method.
     * 
     * @param object
     * @return
     */
    template <typename _T>
    inline bool isInstanceOf(const Class<_T>& rhs) const
    {
        return m_hash == rhs.getTypeHash();
    }

    /**
     * Returns true if this class descriptor is a sub-type of the given
     * class descriptor parameter.
     * <p>
     * This method walks the inheritance graph recursively, and stops if the
     * given super class is found in the super-class graph.
     *
     * @param rhs
     * @return
     */
    template <typename _T>
    inline bool isKindOf(const Class<_T>& rhs) const
    {
        bool result = false;
        for (unsigned i = 0; i < m_superTypes.size() && result == false; ++i)
        {
            if (m_superTypes.at(i) != NULL)
                result = (walkInheritanceGraph(rhs.getName(), (const Class<T>*) m_superTypes.at(i)) != NULL);
        }
        return result;
    }

private:

    std::vector<const bits::Type*> m_superTypes;

    /**
     * This recursive function searches within the inheritance graph for a given
     * class descriptor reference. It returns a pointer to the polymorphic type.
     * 
     * @param className
     * @return
     */
    inline const bits::Type* walkInheritanceGraph(const char* className,
                                                  const Class<T>* current) const
    {
        // Base case: the current walked class is the same type we're looking for
        int typeHash = encodeTypeName(className);
        const bits::Type* result = (typeHash == current->getTypeHash()) ? current : NULL;

        // Recursive case: walk this function for every super class
        for (unsigned i = 0; i < current->m_superTypes.size() && result == NULL; ++i)
        {
            if (current->m_superTypes.at(i) != NULL)
                result = walkInheritanceGraph(className, (const Class<T>*) current->m_superTypes.at(i));
        }
        return result;
    }

} ;

namespace reflection
{

/**
 * Casts a type reference to a class reference. The type safety of the
 * operation is not ensured.
 * <p>
 * A safer version of this function checks the type hashes.
 *
 * @return
 */
template <typename _T>
const Class<_T>& asClassUnsafe(const bits::Type& rhs)
{
    return static_cast<const Class<_T>&> (rhs);
}

/**
 * A safer version of the unsafe class cast. The second parameter is a
 * C string representing the expected class name. The provided class name
 * must be a fully qualified name for this method to work.
 * <p>
 *
 */
template <typename _T>
const Class<_T>& asClass(const bits::Type& rhs, const char* expectedClass)
{
    if (rhs.getTypeHash() != bits::Type::encodeTypeName(expectedClass))
    {
        char exceptionMessage[128] = {0};
        std::sprintf(exceptionMessage, "invalid class cast, expected '%s' or valid covariant type, got '%s' instead (contravariant type).", expectedClass, rhs.getName());

        throw ClassCastException(exceptionMessage);
    }
    return asClassUnsafe<_T>(rhs);
}

/**
 * Returns the runtime polymorphic type of an object. This method is safe because
 * it uses type inference by the compiler to determine the correct <code>Class</code>
 * template instance that must be returned.
 * <p>
 * This method does not throw any exceptions.
 * 
 * @param rhs
 * @return
 */
template <typename _T>
const Class<_T>& getClass(const _T& rhs)
{
    return rhs.template getClass<_T>();
}

namespace bits
{

/**
 * Returns true whether an object can be casted into the target type.
 *
 * @param object
 * @return
 */
template <typename _T, typename _E>
bool _is_casteable(const _E& object)
{
    return (getClass(object).isKindOf(_T::getCompileTimeClass()));
}

}

/**
 * Casts an expression or value to a new defined type. The cast is runtime type
 * checked, therefore, it is a little bit slower than primitive static_cast.
 * <p>
 * However, the cast is useful with polymorphic objects. In all respects it is
 * equivalent to the C++ default dynamic cast.
 * <p>
 * 
 * @return
 */
template <typename _T, typename _E>
_T& runtime_cast(_E& object)
{
    if (bits::_is_casteable<_T>(object) == false)
    {
        char exceptionMessage[1024] = {0};
        std::sprintf(exceptionMessage, "invalid dynamic cast, '%s' is not a polymorphic covariant of '%s'.",
                     _E::getCompileTimeClass().getName(),
                     _T::getCompileTimeClass().getName());

        throw ClassCastException(exceptionMessage);
    }
    return static_cast<_T&> (object);
}

}
}
}

#endif /* CLASS_H */

