/*	NAME:
		TBundle.cpp

	DESCRIPTION:
		NBundle tests.

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
#include "NBundle.h"
#include "NTestFixture.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Paths
#if NN_TARGET_MACOS
static const NString kPathBundle                            = "/Applications/Safari.app";
#else
static const NString kPathBundle                            = "";
#endif





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TBundle)
{
	NBundle theBundle;

	SETUP
	{
		// Mac-specific setup
		if (!kPathBundle.IsEmpty())
		{
			theBundle = NBundle(NFile(kPathBundle));
		}
	}
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBundle, "Default")
{


	// Perform the test
	REQUIRE(theBundle.IsValid());

	NString theString = theBundle.GetIdentifier();
	if (!kPathBundle.IsEmpty())
	{
		REQUIRE(!theString.IsEmpty());
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBundle, "Files")
{


	// Perform the test
	if (!kPathBundle.IsEmpty())
	{
		NFile theFile = theBundle.GetRoot();
		REQUIRE(theFile.IsDirectory());

		theFile = theBundle.GetResources();
		REQUIRE(theFile.IsDirectory());

		theFile = theBundle.GetExecutable();
		REQUIRE(theFile.IsFile());
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBundle, "Info.plist")
{


	// Perform the test
	if (!kPathBundle.IsEmpty())
	{
		NDictionary theInfo = theBundle.GetInfoDictionary();
		REQUIRE(!theInfo.IsEmpty());
	}
}
