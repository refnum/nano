/*	NAME:
		TStdContainer.cpp

	DESCRIPTION:
		NSTLUtilities tests.

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
#include "NStdContainer.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr uint32_t kTestArray[]                      = {1, 2, 3};





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TStdContainer){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStdContainer, "nstd:vector/C")
{


	// Perform the test
	auto theResult = nstd::vector(kTestArray);
	REQUIRE(theResult.size() == std::size(kTestArray));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStdContainer, "nstd:vector/Pointer")
{


	// Perform the test
	auto theResult = nstd::vector(std::size(kTestArray), &kTestArray[0]);
	REQUIRE(theResult.size() == std::size(kTestArray));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStdContainer, "nstd:vector/List")
{


	// Perform the test
	uint32_t a = 1;
	uint32_t b = 1;
	uint32_t c = 1;

	auto theResult = nstd::vector<uint32_t>(a, b, c);
	REQUIRE(theResult.size() == 3);

	theResult = nstd::vector<uint32_t>(1, 2, 3, 4);
	REQUIRE(theResult.size() == 4);
}
