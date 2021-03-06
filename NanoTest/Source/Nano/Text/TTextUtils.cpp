/*	NAME:
		TTextUtils.cpp

	DESCRIPTION:
		Text utilities.

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
// Nano
#include "NData.h"
#include "NTestFixture.h"
#include "NTextUtils.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const NVectorString kTestStrings                     = {"One", "Two", "", "", "Three", "Four", ""};
static const uint8_t       kTestBytes[]                     = {0xA1, 0xB2, 0xC3, 0xD4};
static const NData         kTestData(sizeof(kTestBytes), kTestBytes);





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(NTextUtils){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(NTextUtils, "ToLower")
{


	// Perform the test
	REQUIRE(NTextUtils::GetLower(U'A') == U'a');
	REQUIRE(NTextUtils::GetLower(U'Z') == U'z');
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(NTextUtils, "ToUpper")
{


	// Perform the test
	REQUIRE(NTextUtils::GetUpper(U'a') == U'A');
	REQUIRE(NTextUtils::GetUpper(U'z') == U'Z');
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(NTextUtils, "IsDigit")
{


	// Perform the test
	REQUIRE(NTextUtils::IsDigit(U'0'));
	REQUIRE(NTextUtils::IsDigit(U'1'));
	REQUIRE(NTextUtils::IsDigit(U'2'));
	REQUIRE(NTextUtils::IsDigit(U'3'));
	REQUIRE(NTextUtils::IsDigit(U'4'));
	REQUIRE(NTextUtils::IsDigit(U'5'));
	REQUIRE(NTextUtils::IsDigit(U'6'));
	REQUIRE(NTextUtils::IsDigit(U'7'));
	REQUIRE(NTextUtils::IsDigit(U'8'));
	REQUIRE(NTextUtils::IsDigit(U'9'));

	REQUIRE(!NTextUtils::IsDigit(U'A'));
	REQUIRE(!NTextUtils::IsDigit(U'Z'));
	REQUIRE(!NTextUtils::IsDigit(U'a'));
	REQUIRE(!NTextUtils::IsDigit(U'z'));
	REQUIRE(!NTextUtils::IsDigit(U'!'));
	REQUIRE(!NTextUtils::IsDigit(U'?'));
	REQUIRE(!NTextUtils::IsDigit(U'#'));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(NTextUtils, "IsAlpha")
{


	// Perform the test
	REQUIRE(NTextUtils::IsAlpha(U'A'));
	REQUIRE(NTextUtils::IsAlpha(U'Z'));
	REQUIRE(NTextUtils::IsAlpha(U'a'));
	REQUIRE(NTextUtils::IsAlpha(U'z'));

	REQUIRE(!NTextUtils::IsAlpha(U'0'));
	REQUIRE(!NTextUtils::IsAlpha(U'9'));
	REQUIRE(!NTextUtils::IsAlpha(U'!'));
	REQUIRE(!NTextUtils::IsAlpha(U'?'));
	REQUIRE(!NTextUtils::IsAlpha(U'#'));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(NTextUtils, "IsSentence")
{


	// Perform the test
	REQUIRE(NTextUtils::IsSentence(U'!'));
	REQUIRE(NTextUtils::IsSentence(U'.'));
	REQUIRE(NTextUtils::IsSentence(U'?'));

	REQUIRE(!NTextUtils::IsSentence(U'0'));
	REQUIRE(!NTextUtils::IsSentence(U'9'));
	REQUIRE(!NTextUtils::IsSentence(U'A'));
	REQUIRE(!NTextUtils::IsSentence(U'Z'));
	REQUIRE(!NTextUtils::IsSentence(U'a'));
	REQUIRE(!NTextUtils::IsSentence(U'z'));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(NTextUtils, "GetHex")
{


	// Perform the test
	REQUIRE(NTextUtils::GetHex(kTestData) == "a1b2c3d4");
	REQUIRE(NTextUtils::GetHex(std::size(kTestBytes), kTestBytes) == "a1b2c3d4");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(NTextUtils, "RemoveEmpty")
{


	// Perform the test
	NVectorString theResult = NTextUtils::RemoveEmpty(kTestStrings);
	REQUIRE(theResult.size() == 4);
	REQUIRE(theResult[0] == "One");
	REQUIRE(theResult[1] == "Two");
	REQUIRE(theResult[2] == "Three");
	REQUIRE(theResult[3] == "Four");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(NTextUtils, "Join")
{


	// Perform the test
	NString theResult = NTextUtils::Join(kTestStrings, ",");
	REQUIRE(theResult == "One,Two,,,Three,Four,");

	theResult = NTextUtils::Join(NTextUtils::RemoveEmpty(kTestStrings), ",");
	REQUIRE(theResult == "One,Two,Three,Four");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(NTextUtils, "Repeat")
{


	// Perform the test
	NString theResult = NTextUtils::Repeat(10, "*");
	REQUIRE(theResult == "**********");

	theResult = NTextUtils::Repeat(12, "A B");
	REQUIRE(theResult == "A BA BA BA BA BA BA BA BA BA BA BA B");
}
