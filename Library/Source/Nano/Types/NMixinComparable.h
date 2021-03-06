/*	NAME:
		NMixinComparable.h

	DESCRIPTION:
		Mix-in for comparable objects.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
#ifndef NMIXIN_COMPARABLE_H
#define NMIXIN_COMPARABLE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoTypes.h"

// System
#include <string.h>
#include <algorithm>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
enum class NComparison
{
	LessThan,
	EqualTo,
	GreaterThan
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<class T>
class NMixinComparable
{
public:
	// Operators
	constexpr bool                      operator==(const T& theValue) const;
	constexpr bool                      operator!=(const T& theValue) const;
	constexpr bool                      operator<=(const T& theValue) const;
	constexpr bool                      operator<( const T& theValue) const;
	constexpr bool                      operator>=(const T& theValue) const;
	constexpr bool                      operator>( const T& theValue) const;


public:
	// Compare an object
	//
	// Must be implemented by derived classes.
	constexpr bool                      CompareEqual(const T& theValue) const;
	constexpr NComparison               CompareOrder(const T& theValue) const;


private:
	constexpr bool                      IsEqual( const T& theValue) const;
	constexpr NComparison               GetOrder(const T& theValue) const;
};





//=============================================================================
//		Public Functions
//-----------------------------------------------------------------------------
// Compare two types
template<typename A, typename B>
constexpr NComparison NCompare(const A& a, const B& b)
{
	if (a < b)
	{
		return NComparison::LessThan;
	}
	else if (a > b)
	{
		return NComparison::GreaterThan;
	}
	else
	{
		return NComparison::EqualTo;
	}
}


// Convert a memcmp-style comparison result
constexpr NComparison NCompare(int32_t x)
{
	return NCompare(x, 0);
}


// Compare two blocks of data
inline NComparison NCompare(size_t sizeA, const void* dataA, size_t sizeB, const void* dataB)
{


	// Compare the data
	//
	// We check for a common prefix, then break ties by size.
	NComparison theResult = NCompare(memcmp(dataA, dataB, std::min(sizeA, sizeB)));

	if (theResult == NComparison::EqualTo && sizeA != sizeB)
	{
		theResult = NCompare(sizeA, sizeB);
	}

	return theResult;
}





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinComparable.inl"



#endif // NMIXIN_COMPARABLE_H
