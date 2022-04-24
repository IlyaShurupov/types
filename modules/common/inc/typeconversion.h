
#pragma once

////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Code covered by the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "typeselect.h"

namespace compiler {
////////////////////////////////////////////////////////////////////////////////
// class template IsSameType
// Return true iff two given types are the same
// Invocation: SameType<T, U>::value
// where:
// T and U are types
// Result evaluates to true iff U == T (types equal)
////////////////////////////////////////////////////////////////////////////////

  template <typename T, typename U>
  struct IsSameType {
    enum { value = false };
  };

  template <typename T>
  struct IsSameType<T, T> {
    enum { value = true };
  };

////////////////////////////////////////////////////////////////////////////////
// Helper types Small and Big - guarantee that sizeof(Small) < sizeof(Big)
////////////////////////////////////////////////////////////////////////////////

  namespace Private {
    template <class T, class U>
    struct ConversionHelper {
      typedef char Small;
      struct Big { char dummy[2]; };
      static Big   Test(...);
      static Small Test(U);
      static T MakeT();
    };
  }

////////////////////////////////////////////////////////////////////////////////
// class template Conversion
// Figures out the conversion relationships between two types
// Invocations (T and U are types):
// a) Conversion<T, U>::exists
// returns (at compile time) true if there is an implicit conversion from T
// to U (example: Derived to Base)
// b) Conversion<T, U>::exists2Way
// returns (at compile time) true if there are both conversions from T
// to U and from U to T (example: int to char and back)
// c) Conversion<T, U>::sameType
// returns (at compile time) true if T and U represent the same type
//
// Caveat: might not work if T and U are in a private inheritance hierarchy.
////////////////////////////////////////////////////////////////////////////////

  template <class T, class U>
  struct Conversion {
    typedef Private::ConversionHelper<T, U> H;
    #ifndef __MWERKS__
    enum { exists = sizeof(typename H::Small) == sizeof((H::Test(H::MakeT()))) };
    #else
    enum { exists = false };
    #endif
    enum { exists2Way = exists && Conversion<U, T>::exists };
    enum { sameType = false };
  };

  template <class T>
  struct Conversion<T, T> {
    enum { exists = 1, exists2Way = 1, sameType = 1 };
  };

  template <class T>
  struct Conversion<void, T> {
    enum { exists = 0, exists2Way = 0, sameType = 0 };
  };

  template <class T>
  struct Conversion<T, void> {
    enum { exists = 0, exists2Way = 0, sameType = 0 };
  };

  template <>
  struct Conversion<void, void> {
    public:
    enum { exists = 1, exists2Way = 1, sameType = 1 };
  };

////////////////////////////////////////////////////////////////////////////////
// class template SuperSubclass
// Invocation: SuperSubclass<B, D>::value where B and D are types.
// Returns true if B is a public base of D, or if B and D are aliases of the
// same type.
//
// Caveat: might not work if T and U are in a private inheritance hierarchy.
////////////////////////////////////////////////////////////////////////////////

  template <class T, class U>
  struct SuperSubclass {
    enum {
      value = (::compiler::Conversion<const volatile U*, const volatile T*>::exists &&
      !::compiler::Conversion<const volatile T*, const volatile void*>::sameType)
    };

// Dummy enum to make sure that both classes are fully defined.
    enum { dontUseWithIncompleteTypes = (sizeof(T) == sizeof(U)) };
  };

  template <>
  struct SuperSubclass<void, void> {
    enum { value = false };
  };

  template <class U>
  struct SuperSubclass<void, U> {
    enum {
      value = (::compiler::Conversion<const volatile U*, const volatile void*>::exists &&
      !::compiler::Conversion<const volatile void*, const volatile void*>::sameType)
    };

// Dummy enum to make sure that both classes are fully defined.
    enum { dontUseWithIncompleteTypes = (0 == sizeof(U)) };
  };

  template <class T>
  struct SuperSubclass<T, void> {
    enum {
      value = (::compiler::Conversion<const volatile void*, const volatile T*>::exists &&
      !::compiler::Conversion<const volatile T*, const volatile void*>::sameType)
    };

// Dummy enum to make sure that both classes are fully defined.
    enum { dontUseWithIncompleteTypes = (sizeof(T) == 0) };
  };

  ////////////////////////////////////////////////////////////////////////////////
  // class template SuperSubclassStrict
  // Invocation: SuperSubclassStrict<B, D>::value where B and D are types.
  // Returns true if B is a public base of D.
  //
  // Caveat: might not work if T and U are in a private inheritance hierarchy.
  ////////////////////////////////////////////////////////////////////////////////

  template<class T, class U>
  struct SuperSubclassStrict {
    enum {
      value = (::compiler::Conversion<const volatile U*, const volatile T*>::exists &&
      !::compiler::Conversion<const volatile T*, const volatile void*>::sameType &&
      !::compiler::Conversion<const volatile T*, const volatile U*>::sameType)
    };

// Dummy enum to make sure that both classes are fully defined.
    enum { dontUseWithIncompleteTypes = (sizeof(T) == sizeof(U)) };
  };

  template<>
  struct SuperSubclassStrict<void, void> {
    enum { value = false };
  };

  template<class U>
  struct SuperSubclassStrict<void, U> {
    enum {
      value = (::compiler::Conversion<const volatile U*, const volatile void*>::exists &&
      !::compiler::Conversion<const volatile void*, const volatile void*>::sameType &&
      !::compiler::Conversion<const volatile void*, const volatile U*>::sameType)
    };

// Dummy enum to make sure that both classes are fully defined.
    enum { dontUseWithIncompleteTypes = (0 == sizeof(U)) };
  };

  template<class T>
  struct SuperSubclassStrict<T, void> {
    enum {
      value = (::compiler::Conversion<const volatile void*, const volatile T*>::exists &&
      !::compiler::Conversion<const volatile T*, const volatile void*>::sameType &&
      !::compiler::Conversion<const volatile T*, const volatile void*>::sameType)
    };

// Dummy enum to make sure that both classes are fully defined.
    enum { dontUseWithIncompleteTypes = (sizeof(T) == 0) };
  };


}   // namespace compiler

////////////////////////////////////////////////////////////////////////////////
// macro SUPERSUBCLASS
// Invocation: SUPERSUBCLASS(B, D) where B and D are types.
// Returns true if B is a public base of D, or if B and D are aliases of the
// same type.
//
// Caveat: might not work if T and U are in a private inheritance hierarchy.
// Deprecated: Use SuperSubclass class template instead.
////////////////////////////////////////////////////////////////////////////////

#define SUPERSUBCLASS(T, U) ::compiler::SuperSubclass<T,U>::value

////////////////////////////////////////////////////////////////////////////////
// macro SUPERSUBCLASS_STRICT
// Invocation: SUPERSUBCLASS(B, D) where B and D are types.
// Returns true if B is a public base of D.
//
// Caveat: might not work if T and U are in a private inheritance hierarchy.
// Deprecated: Use SuperSubclassStrict class template instead.
////////////////////////////////////////////////////////////////////////////////

#define SUPERSUBCLASS_STRICT(T, U) ::compiler::SuperSubclassStrict<T,U>::value
