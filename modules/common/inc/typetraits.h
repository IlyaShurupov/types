
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

#include <Typelist.h>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4180 ) //qualifier applied to function type has no meaning; ignored
#endif

namespace compiler {
////////////////////////////////////////////////////////////////////////////////
// class template IsCustomUnsignedInt
// Offers a means to integrate nonstandard built-in unsigned integral types
// (such as unsigned __int64 or unsigned long long int) with the TypeTraits
//     class template defined below.
// Invocation: IsCustomUnsignedInt<T> where T is any type
// Defines 'value', an enum that is 1 iff T is a custom built-in unsigned
//     integral type
// Specialize this class template for nonstandard unsigned integral types
//     and define value = 1 in those specializations
////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	struct IsCustomUnsignedInt {
		enum { value = 0 };
	};

////////////////////////////////////////////////////////////////////////////////
// class template IsCustomSignedInt
// Offers a means to integrate nonstandard built-in unsigned integral types
// (such as unsigned __int64 or unsigned long long int) with the TypeTraits
//     class template defined below.
// Invocation: IsCustomSignedInt<T> where T is any type
// Defines 'value', an enum that is 1 iff T is a custom built-in signed
//     integral type
// Specialize this class template for nonstandard unsigned integral types
//     and define value = 1 in those specializations
////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	struct IsCustomSignedInt {
		enum { value = 0 };
	};

////////////////////////////////////////////////////////////////////////////////
// class template IsCustomFloat
// Offers a means to integrate nonstandard floating point types with the
//     TypeTraits class template defined below.
// Invocation: IsCustomFloat<T> where T is any type
// Defines 'value', an enum that is 1 iff T is a custom built-in
//     floating point type
// Specialize this class template for nonstandard unsigned integral types
//     and define value = 1 in those specializations
////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	struct IsCustomFloat {
		enum { value = 0 };
	};

////////////////////////////////////////////////////////////////////////////////
// Helper types for class template TypeTraits defined below
////////////////////////////////////////////////////////////////////////////////

	namespace Private {

		typedef compiler::Seq<unsigned char, unsigned short int, unsigned int, unsigned long int>::list StdUnsignedInts;
		typedef compiler::Seq<signed char, short int, int, long int>::list StdSignedInts;
		typedef compiler::Seq<bool, char, wchar_t>::list StdOtherInts;
		typedef compiler::Seq<float, double, long double>::list StdFloats;

		template <typename U> struct AddPointer { typedef U* Result; };
		template <typename U> struct AddPointer<U&> { typedef U* Result; };

		template <class U> struct AddReference { typedef U& Result; };
		template <class U> struct AddReference<U&> { typedef U& Result; };
		template <> struct AddReference<void> { typedef NullType Result; };

		template <class U> struct AddParameterType { typedef const U& Result; };
		template <class U> struct AddParameterType<U&> { typedef U& Result; };
		template <> struct AddParameterType<void> { typedef NullType Result; };

	}// namespace Private

////////////////////////////////////////////////////////////////////////////////
// class template TypeTraits
//
// Figures out at compile time various properties of any given type
// Invocations (T is a type, TypeTraits<T>::Property):
//
// - isPointer       : returns true if T is a pointer type
// - PointeeType     : returns the type to which T points if T is a pointer
//                     type, NullType otherwise
// - isReference     : returns true if T is a reference type
// - ReferredType    : returns the type to which T refers if T is a reference
//                     type, NullType otherwise
// - isMemberPointer : returns true if T is a pointer to member type
// - isStdUnsignedInt: returns true if T is a standard unsigned integral type
// - isStdSignedInt  : returns true if T is a standard signed integral type
// - isStdIntegral   : returns true if T is a standard integral type
// - isStdFloat      : returns true if T is a standard floating-point type
// - isStdArith      : returns true if T is a standard arithmetic type
// - isStdFundamental: returns true if T is a standard fundamental type
// - isUnsignedInt   : returns true if T is a unsigned integral type
// - isSignedInt     : returns true if T is a signed integral type
// - isIntegral      : returns true if T is a integral type
// - isFloat         : returns true if T is a floating-point type
// - isArith         : returns true if T is a arithmetic type
// - isFundamental   : returns true if T is a fundamental type
// - ParameterType   : returns the optimal type to be used as a parameter for
//                     functions that take Ts
// - isConst         : returns true if T is a const-qualified type
// - NonConstType    : Type with removed 'const' qualifier from T, if any
// - isVolatile      : returns true if T is a volatile-qualified type
// - NonVolatileType : Type with removed 'volatile' qualifier from T, if any
// - UnqualifiedType : Type with removed 'const' and 'volatile' qualifiers from
//                     T, if any
// - ParameterType   : returns the optimal type to be used as a parameter
//                       for functions that take 'const T's
//
////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	class TypeTraits {
		private:

		template <class U> struct ReferenceTraits {
			enum { result = false };
			typedef U ReferredType;
		};

		template <class U> struct ReferenceTraits<U&> {
			enum { result = true };
			typedef U ReferredType;
		};

		template <class U> struct PointerTraits {
			enum { result = false };
			typedef NullType PointeeType;
		};

		template <class U> struct PointerTraits<U*> {
			enum { result = true };
			typedef U PointeeType;
		};

		template <class U> struct PointerTraits<U*&> {
			enum { result = true };
			typedef U PointeeType;
		};

		template <class U> struct PToMTraits {
			enum { result = false };
		};

		template <class U, class V> struct PToMTraits<U V::*> {
			enum { result = true };
		};

		template <class U, class V> struct PToMTraits<U V::*&> {
			enum { result = true };
		};

		template <class U> struct UnConst {
			typedef U Result;
			enum { isConst = 0 };
		};

		template <class U> struct UnConst<const U> {
			typedef U Result;
			enum { isConst = 1 };
		};

		template <class U> struct UnConst<const U&> {
			typedef U& Result;
			enum { isConst = 1 };
		};

		template <class U> struct UnVolatile {
			typedef U Result;
			enum { isVolatile = 0 };
		};

		template <class U> struct UnVolatile<volatile U> {
			typedef U Result;
			enum { isVolatile = 1 };
		};

		template <class U> struct UnVolatile<volatile U&> {
			typedef U& Result;
			enum { isVolatile = 1 };
		};

		public:
		typedef typename UnConst<T>::Result
			NonConstType;
		typedef typename UnVolatile<T>::Result
			NonVolatileType;
		typedef typename UnVolatile<typename UnConst<T>::Result>::Result
			UnqualifiedType;
		typedef typename PointerTraits<UnqualifiedType>::PointeeType
			PointeeType;
		typedef typename ReferenceTraits<T>::ReferredType
			ReferredType;

		enum { isConst = UnConst<T>::isConst };
		enum { isVolatile = UnVolatile<T>::isVolatile };
		enum { isReference = ReferenceTraits<UnqualifiedType>::result };
		enum { isMemberPointer = PToMTraits<typename ReferenceTraits<UnqualifiedType>::ReferredType >::result };
		enum { isPointer = PointerTraits<typename ReferenceTraits<UnqualifiedType>::ReferredType >::result };

		enum {
			isStdUnsignedInt = TL::IndexOf<Private::StdUnsignedInts, UnqualifiedType>::value >= 0 ||
			TL::IndexOf<Private::StdUnsignedInts,
			typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0
		};
		enum {
			isStdSignedInt = TL::IndexOf<Private::StdSignedInts, UnqualifiedType>::value >= 0 ||
			TL::IndexOf<Private::StdSignedInts,
			typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0
		};
		enum {
			isStdIntegral = isStdUnsignedInt || isStdSignedInt ||
			TL::IndexOf<Private::StdOtherInts, UnqualifiedType>::value >= 0 ||
			TL::IndexOf<Private::StdOtherInts,
			typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0
		};
		enum {
			isStdFloat = TL::IndexOf<Private::StdFloats, UnqualifiedType>::value >= 0 ||
			TL::IndexOf<Private::StdFloats,
			typename ReferenceTraits<UnqualifiedType>::ReferredType>::value >= 0
		};

		enum { isStdArith = isStdIntegral || isStdFloat };
		enum { isStdFundamental = isStdArith || isStdFloat || Conversion<T, void>::sameType };

		enum { isUnsignedInt = isStdUnsignedInt || IsCustomUnsignedInt<UnqualifiedType>::value };
		enum { isSignedInt = isStdSignedInt || IsCustomSignedInt<UnqualifiedType>::value };
		enum { isIntegral = isStdIntegral || isUnsignedInt || isSignedInt };
		enum { isFloat = isStdFloat || IsCustomFloat<UnqualifiedType>::value };
		enum { isArith = isIntegral || isFloat };
		enum { isFundamental = isStdFundamental || isArith };

		typedef typename Select<isStdArith || isPointer || isMemberPointer, T, typename Private::AddParameterType<T>::Result>::Result
			ParameterType;
	};
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER