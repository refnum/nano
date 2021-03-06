/*	NAME:
		NCFNumber.cpp

	DESCRIPTION:
		CFNumberRef wrapper.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCFNumber.h"





//=============================================================================
//		NCFNumber::GetNumber : Get the number.
//-----------------------------------------------------------------------------
NNumber NCFNumber::GetNumber() const
{


	// Get the number
	CFNumberRef cfNumber = *this;
	NNumber     theNumber;

	if (cfNumber != nullptr)
	{
		if (CFNumberIsFloatType(cfNumber))
		{
			float64_t valueFloat64 = 0;

			bool wasOK = CFNumberGetValue(cfNumber, kCFNumberFloat64Type, &valueFloat64);
			NN_REQUIRE(wasOK);

			theNumber.SetValue(valueFloat64);
		}
		else
		{
			int64_t valueInt64 = 0;

			bool wasOK = CFNumberGetValue(cfNumber, kCFNumberSInt64Type, &valueInt64);
			NN_REQUIRE(wasOK);

			theNumber.SetValue(valueInt64);
		}
	}

	return theNumber;
}





//=============================================================================
//		NCFNumber::SetNumber : Set the number.
//-----------------------------------------------------------------------------
bool NCFNumber::SetNumber(const NNumber& theNumber)
{


	// Set the number
	bool wasOK = false;

	if (theNumber.IsReal())
	{
		float64_t valueFloat64 = theNumber.GetFloat64();
		wasOK = Assign(CFNumberCreate(kCFAllocatorDefault, kCFNumberFloat64Type, &valueFloat64));
	}
	else
	{
		int64_t valueInt64 = theNumber.GetInt64();
		wasOK = Assign(CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt64Type, &valueInt64));
	}

	return wasOK;
}
