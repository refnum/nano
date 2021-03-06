/*	NAME:
		TCFData.cpp

	DESCRIPTION:
		NCFData tests.

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
#include "NCFData.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const uint8_t kTestBytes[]                           = {1, 2, 3, 4};

static const NData kTestData(sizeof(kTestBytes), kTestBytes);





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TCFData){};

NANO_FIXTURE(TCFMutableData){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCFData, "Default")
{


	// Perform the test
	NCFData cfData;

	REQUIRE(!cfData.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCFData, "Constructor")
{


	// Perform the test
	NCFData cfData(kTestData);

	REQUIRE(cfData.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCFData, "GetData/Copy")
{


	// Perform the test
	NCFData cfData(kTestData);

	NData theData = cfData.GetData(NCFSource::Copy);
	REQUIRE(theData.GetData() != CFDataGetBytePtr(cfData));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCFData, "GetData/View")
{


	// Perform the test
	NCFData cfData(kTestData);

	NData theData = cfData.GetData(NCFSource::View);
	REQUIRE(theData.GetData() == CFDataGetBytePtr(cfData));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCFData, "SetData/Copy")
{


	// Perform the test
	NCFData cfData;

	bool wasOK = cfData.SetData(kTestData, NCFSource::Copy);
	REQUIRE(wasOK);
	REQUIRE(kTestData.GetData() != CFDataGetBytePtr(cfData));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCFData, "SetData/View")
{


	// Perform the test
	NCFData cfData;

	bool wasOK = cfData.SetData(kTestData, NCFSource::View);
	REQUIRE(wasOK);
	REQUIRE(kTestData.GetData() == CFDataGetBytePtr(cfData));
}





#pragma mark TCFMutableData
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCFMutableData, "Default")
{


	// Perform the test
	NCFMutableData cfMutableData;

	REQUIRE(!cfMutableData.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCFMutableData, "Constructor")
{


	// Perform the test
	NCFMutableData cfMutableData(kTestData);

	REQUIRE(cfMutableData.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCFMutableData, "GetData/Copy")
{


	// Perform the test
	NCFMutableData cfMutableData(kTestData);

	NData theData = cfMutableData.GetData(NCFSource::Copy);
	REQUIRE(theData.GetData() != CFDataGetBytePtr(cfMutableData));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCFMutableData, "GetData/View")
{


	// Perform the test
	NCFMutableData cfMutableData(kTestData);

	NData theData = cfMutableData.GetData(NCFSource::View);
	REQUIRE(theData.GetData() == CFDataGetBytePtr(cfMutableData));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCFMutableData, "SetData")
{


	// Perform the test
	NCFMutableData cfMutableData;

	bool wasOK = cfMutableData.SetData(kTestData);
	REQUIRE(wasOK);
	REQUIRE(kTestData.GetData() != CFDataGetBytePtr(cfMutableData));
}
