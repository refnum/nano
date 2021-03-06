/*	NAME:
		TFileInfo.cpp

	DESCRIPTION:
		NFileInfo tests.

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
#include "NFileInfo.h"
#include "NFormat.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
#if NN_TARGET_WINDOWS
static const NFilePath kPathFile                            = NString("c:\\Windows\\regedit.exe");
static const NFilePath kPathDirectory                       = NString("c:\\Windows\\System");
static const NFilePath kPathDoesNotExist                    = NString("c:\\63785644-da36-4148-939f-4416cb5ea56e");
#else
static const NFilePath kPathFile                            = NString("/bin/bash");
static const NFilePath kPathDirectory                       = NString("/tmp");
static const NFilePath kPathDoesNotExist                    = NString("/63785644-da36-4148-939f-4416cb5ea56e");
#endif





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFileInfo)
{
	NFileInfo theInfo;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Default")
{


	// Perform the test
	REQUIRE(!theInfo.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Constructor")
{


	// Perform the test
	REQUIRE(NFileInfo(kPathFile).IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Clear")
{


	// Perform the test
	theInfo = NFileInfo(kPathFile);
	REQUIRE(theInfo.IsValid());

	theInfo.Clear();
	REQUIRE(!theInfo.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "SetPath")
{


	// Perform the test
	REQUIRE(theInfo.GetPath() != kPathFile);
	theInfo.SetPath(kPathFile);
	REQUIRE(theInfo.GetPath() == kPathFile);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Refresh")
{


	// Perform the test
	theInfo.SetPath(kPathFile);
	theInfo.Refresh();
	REQUIRE(theInfo.GetPath() == kPathFile);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Status")
{


	// Perform the test
	theInfo.SetPath(kPathFile);
	REQUIRE(theInfo.Exists());
	REQUIRE(theInfo.IsFile());
	REQUIRE(!theInfo.IsDirectory());

	theInfo.SetPath(kPathDirectory);
	REQUIRE(theInfo.Exists());
	REQUIRE(!theInfo.IsFile());
	REQUIRE(theInfo.IsDirectory());

	theInfo.SetPath(kPathDoesNotExist);
	REQUIRE(!theInfo.Exists());
	REQUIRE(!theInfo.IsFile());
	REQUIRE(!theInfo.IsDirectory());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Permission")
{


	// Perform the test
	theInfo.SetPath(kPathFile);
	REQUIRE(theInfo.CanRead());
	REQUIRE((!theInfo.CanWrite() || NN_TARGET_WINDOWS));
	REQUIRE(theInfo.CanExecute());

	theInfo.SetPath(kPathDirectory);
	REQUIRE(theInfo.CanRead());
	REQUIRE(theInfo.CanWrite());
	REQUIRE(theInfo.CanExecute());

	theInfo.SetPath(kPathDoesNotExist);
	REQUIRE(!theInfo.CanRead());
	REQUIRE(!theInfo.CanWrite());
	REQUIRE(!theInfo.CanExecute());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Time")
{


	// Perform the test
	theInfo.SetPath(kPathFile);
	REQUIRE(theInfo.GetCreationTime() != 0.0);
	REQUIRE(theInfo.GetModifiedTime() != 0.0);
	REQUIRE(theInfo.GetModifiedTime() >= theInfo.GetCreationTime());

	theInfo.SetPath(kPathDirectory);
	REQUIRE(theInfo.GetCreationTime() != 0.0);
	REQUIRE(theInfo.GetModifiedTime() != 0.0);
	REQUIRE(theInfo.GetModifiedTime() >= theInfo.GetCreationTime());

	theInfo.SetPath(kPathDoesNotExist);
	REQUIRE(theInfo.GetCreationTime() == 0.0);
	REQUIRE(theInfo.GetModifiedTime() == 0.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "GetFileSize")
{


	// Perform the test
	theInfo.SetPath(kPathFile);
	REQUIRE(theInfo.GetFileSize() != 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", kPathFile) == kPathFile.GetPath());
}
