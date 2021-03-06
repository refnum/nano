/*	NAME:
		TString.cpp

	DESCRIPTION:
		NString tests.

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
#include "NFormat.h"
#include "NString.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const uint8_t kTestDataSmallUTF8[] =
	{0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};

static const uint16_t kTestDataLargeUTF16[]                 = {
	0x0048, 0x0065, 0x006C, 0x006C, 0x006F, 0x0020, 0x0057, 0x006F, 0x0072, 0x006C, 0x0064,
	0x0020, 0x0054, 0x0065, 0x0073, 0x0074, 0x0069, 0x006E, 0x0067, 0x0020, 0x0054, 0x0065,
	0x0073, 0x0074, 0x0069, 0x006E, 0x0067, 0x0020, 0x004F, 0x006E, 0x0065, 0x0020, 0x0054,
	0x0077, 0x006F, 0x0020, 0x0054, 0x0068, 0x0072, 0x0065, 0x0065};

static const uint32_t kTestDataLargeUTF32[]                 = {
	0x000048, 0x000065, 0x00006C, 0x00006C, 0x00006F, 0x000020, 0x000057, 0x00006F, 0x000072,
	0x00006C, 0x000064, 0x000020, 0x000054, 0x000065, 0x000073, 0x000074, 0x000069, 0x00006E,
	0x000067, 0x000020, 0x000054, 0x000065, 0x000073, 0x000074, 0x000069, 0x00006E, 0x000067,
	0x000020, 0x00004F, 0x00006E, 0x000065, 0x000020, 0x000054, 0x000077, 0x00006F, 0x000020,
	0x000054, 0x000068, 0x000072, 0x000065, 0x000065};

static const NString kTestStringSmall                       = "Hello World";
static const NString kTestStringLarge                       = "Hello World One World Two World Three World";

static const NString kTestStringLines =
	"Lorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
	"Vivamus est erat, scelerisque quis molestie a, bibendum eget risus.\r"
	"Etiam laoreet velit dolor, at eleifend enim ultrices vitae.\r\n"
	"\n"
	"\r"
	"\r\n"
	"Vivamus scelerisque sapien scelerisque ligula.";





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TString)
{
	std::vector<NString> stringObjects;

	SETUP
	{
		stringObjects.push_back(kTestStringSmall);
		stringObjects.push_back(kTestStringLarge);
	}
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Default")
{


	// Perform the test
	NString theString;

	REQUIRE(theString.IsEmpty());
	REQUIRE(theString.GetHash() == 0);
	REQUIRE(theString.GetSize() == 0);

	REQUIRE(theString.GetUTF8() != nullptr);
	REQUIRE(theString.GetUTF16() != nullptr);
	REQUIRE(theString.GetUTF32() != nullptr);

	REQUIRE(theString.GetUTF8()[0] == utf8_t(0));
	REQUIRE(theString.GetUTF16()[0] == utf16_t(0));
	REQUIRE(theString.GetUTF32()[0] == utf32_t(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Container")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(!theString.IsEmpty());
		REQUIRE(theString.GetSize() != 0);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Constructor")
{


	// Perform the test
	NString theString;

	theString = NString(u8"UTF8 Small");
	REQUIRE(theString.GetSize() == 10);

	theString = NString(u8"UTF8 Large Large Large Large Large Large Large");
	REQUIRE(theString.GetSize() == 46);

	theString = NString(u"UTF16 Small");
	REQUIRE(theString.GetSize() == 11);

	theString = NString(u"UTF16 Large Large Large Large Large Large Large");
	REQUIRE(theString.GetSize() == 47);

	theString = NString(U"UTF32 Small");
	REQUIRE(theString.GetSize() == 11);

	theString = NString(U"UTF32 Large Large Large Large Large Large Large");
	REQUIRE(theString.GetSize() == 47);

	theString = NString(NStringEncoding::UTF8, sizeof(kTestDataSmallUTF8), kTestDataSmallUTF8);
	REQUIRE(theString.GetSize() == 11);

	theString = NString(NStringEncoding::UTF16, sizeof(kTestDataLargeUTF16), kTestDataLargeUTF16);
	REQUIRE(theString.GetSize() == 41);

	theString = NString(NStringEncoding::UTF32, sizeof(kTestDataLargeUTF32), kTestDataLargeUTF32);
	REQUIRE(theString.GetSize() == 41);

	theString = NString();
	REQUIRE(theString.GetSize() == 0);


	theString = NString("String Literal");
	REQUIRE(theString.GetSize() == 14);

	const char* localPtr = "Char Pointer";
	theString            = NString(localPtr);
	REQUIRE(theString.GetSize() == 12);

	char charArray[20];
	strncpy(charArray, "Char Array", std::size(charArray));
	theString = NString(charArray);
	REQUIRE(theString.GetSize() == 10);

	std::string stdString("Std String");
	theString = NString(stdString.c_str());
	REQUIRE(theString.GetSize() == 10);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Clear")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(!theString.IsEmpty());
		REQUIRE(theString.GetSize() != 0);

		theString.Clear();

		REQUIRE(theString.IsEmpty());
		REQUIRE(theString.GetSize() == 0);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetText")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.GetText(NStringEncoding::UTF8) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::UTF16) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::UTF32) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::ASCII) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::MacRoman) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::ISOLatin1) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::WindowsLatin1) != nullptr);
	}


	NString     theString     = kTestStringSmall;
	const char* textUTF8      = kTestStringSmall.GetUTF8();
	const void* textASCII     = theString.GetText(NStringEncoding::ASCII);
	const void* textMacRoman  = theString.GetText(NStringEncoding::MacRoman);
	const void* textISOLatin1 = theString.GetText(NStringEncoding::ISOLatin1);
	const void* textWinLatin1 = theString.GetText(NStringEncoding::WindowsLatin1);

	REQUIRE(strcmp(textUTF8, static_cast<const char*>(textASCII)) == 0);
	REQUIRE(strcmp(textUTF8, static_cast<const char*>(textMacRoman)) == 0);
	REQUIRE(strcmp(textUTF8, static_cast<const char*>(textISOLatin1)) == 0);
	REQUIRE(strcmp(textUTF8, static_cast<const char*>(textWinLatin1)) == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetText/Empty")
{


	// Perform the test
	//
	// Empty strings can return encoding from their small buffer.
	NString theString;

	const void* textUTF8      = theString.GetText(NStringEncoding::UTF8);
	const void* textUTF16     = theString.GetText(NStringEncoding::UTF16);
	const void* textUTF32     = theString.GetText(NStringEncoding::UTF32);
	const void* textASCII     = theString.GetText(NStringEncoding::ASCII);
	const void* textMacRoman  = theString.GetText(NStringEncoding::MacRoman);
	const void* textISOLatin1 = theString.GetText(NStringEncoding::ISOLatin1);
	const void* textWinLatin1 = theString.GetText(NStringEncoding::WindowsLatin1);

	REQUIRE(textUTF8 != nullptr);
	REQUIRE(textUTF16 != nullptr);
	REQUIRE(textUTF32 != nullptr);
	REQUIRE(textASCII != nullptr);
	REQUIRE(textMacRoman != nullptr);
	REQUIRE(textISOLatin1 != nullptr);
	REQUIRE(textWinLatin1 != nullptr);

	REQUIRE(static_cast<const utf8_t*>(textUTF8)[0] == utf8_t(0));
	REQUIRE(static_cast<const utf16_t*>(textUTF16)[0] == utf16_t(0));
	REQUIRE(static_cast<const utf32_t*>(textUTF32)[0] == utf32_t(0));
	REQUIRE(static_cast<const char*>(textASCII)[0] == char(0));
	REQUIRE(static_cast<const char*>(textMacRoman)[0] == char(0));
	REQUIRE(static_cast<const char*>(textISOLatin1)[0] == char(0));
	REQUIRE(static_cast<const char*>(textWinLatin1)[0] == char(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetUTF")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.GetUTF8() != nullptr);
		REQUIRE(theString.GetUTF16() != nullptr);
		REQUIRE(theString.GetUTF32() != nullptr);

		REQUIRE(theString.GetText(NStringEncoding::UTF8) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::UTF16) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::UTF32) != nullptr);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetData")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(!theString.GetData(NStringEncoding::UTF8).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::UTF16).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::UTF32).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::ASCII).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::MacRoman).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::ISOLatin1).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::WindowsLatin1).IsEmpty());
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "SetData")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		NString tmpString;
		NData   tmpData;

		tmpData = theString.GetData(NStringEncoding::UTF8);
		tmpString.SetData(NStringEncoding::UTF8, tmpData);
		REQUIRE(tmpString.GetData(NStringEncoding::UTF8) == tmpData);

		tmpData = theString.GetData(NStringEncoding::UTF16);
		tmpString.SetData(NStringEncoding::UTF16, tmpData);
		REQUIRE(tmpString.GetData(NStringEncoding::UTF16) == tmpData);

		tmpData = theString.GetData(NStringEncoding::UTF32);
		tmpString.SetData(NStringEncoding::UTF32, tmpData);
		REQUIRE(tmpString.GetData(NStringEncoding::UTF32) == tmpData);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetContent")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		NStringEncoding theEncoding = NStringEncoding::Unknown;
		size_t          theSize     = 0;
		const void*     theData     = theString.GetContent(&theEncoding, &theSize);

		REQUIRE(theEncoding != NStringEncoding::Unknown);
		REQUIRE(theSize != 0);
		REQUIRE(theData != nullptr);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Find")
{


	// Perform the test
	NRange theRange;

	for (auto theString : stringObjects)
	{
		theRange = theString.Find("World", kNStringNone);
		REQUIRE(theRange == NRange(6, 5));

		theRange = theString.Find("WORLD", kNStringNoCase);
		REQUIRE(theRange == NRange(6, 5));

		theRange = theString.Find("WORLD", kNStringNone);
		REQUIRE(theRange.IsEmpty());
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "FindAll")
{


	// Perform the test
	NVectorRange theRanges;

	for (auto theString : stringObjects)
	{
		theRanges = theString.FindAll("World", kNStringNone);
		if (theString == kTestStringSmall)
		{
			REQUIRE(theRanges.size() == 1);
			REQUIRE(theRanges[0] == NRange(6, 5));
		}
		else
		{
			REQUIRE(theRanges.size() == 4);
			REQUIRE(theRanges[0] == NRange(6, 5));
			REQUIRE(theRanges[1] == NRange(16, 5));
			REQUIRE(theRanges[2] == NRange(26, 5));
			REQUIRE(theRanges[3] == NRange(38, 5));
		}


		theRanges = theString.FindAll("WORLD", kNStringNoCase);
		if (theString == kTestStringSmall)
		{
			REQUIRE(theRanges.size() == 1);
			REQUIRE(theRanges[0] == NRange(6, 5));
		}
		else
		{
			REQUIRE(theRanges.size() == 4);
			REQUIRE(theRanges[0] == NRange(6, 5));
			REQUIRE(theRanges[1] == NRange(16, 5));
			REQUIRE(theRanges[2] == NRange(26, 5));
			REQUIRE(theRanges[3] == NRange(38, 5));
		}


		theRanges = theString.FindAll("WORLD", kNStringNone);
		REQUIRE(theRanges.empty());
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "FindMatch")
{


	// Perform the test
	NPatternMatch patternMatch;

	for (auto theString : stringObjects)
	{
		patternMatch = theString.FindMatch("\\sWo(\\w+)");
		if (theString == kTestStringSmall)
		{
			REQUIRE(patternMatch.theGroups.size() == 1);
			REQUIRE(patternMatch.thePattern == NRange(5, 6));
			REQUIRE(patternMatch.theGroups[0] == NRange(8, 3));
		}
		else
		{
			REQUIRE(patternMatch.theGroups.size() == 1);
			REQUIRE(patternMatch.thePattern == NRange(5, 6));
			REQUIRE(patternMatch.theGroups[0] == NRange(8, 3));
		}


		patternMatch = theString.FindMatch("\\sWO(\\w+)", kNStringNoCase);
		if (theString == kTestStringSmall)
		{
			REQUIRE(patternMatch.theGroups.size() == 1);
			REQUIRE(patternMatch.thePattern == NRange(5, 6));
			REQUIRE(patternMatch.theGroups[0] == NRange(8, 3));
		}
		else
		{
			REQUIRE(patternMatch.theGroups.size() == 1);
			REQUIRE(patternMatch.thePattern == NRange(5, 6));
			REQUIRE(patternMatch.theGroups[0] == NRange(8, 3));
		}


		patternMatch = theString.FindMatch("\\sWO(\\w+)");
		REQUIRE(patternMatch.thePattern.IsEmpty());
		REQUIRE(patternMatch.theGroups.empty());
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "FindMatches")
{


	// Perform the test
	NVectorPatternMatch patternMatches;

	for (auto theString : stringObjects)
	{
		patternMatches = theString.FindMatches("\\sWo(\\w+)");
		if (theString == kTestStringSmall)
		{
			REQUIRE(patternMatches.size() == 1);
			REQUIRE(patternMatches[0].theGroups.size() == 1);
			REQUIRE(patternMatches[0].thePattern == NRange(5, 6));
			REQUIRE(patternMatches[0].theGroups[0] == NRange(8, 3));
		}
		else
		{
			REQUIRE(patternMatches.size() == 4);
			REQUIRE(patternMatches[0].theGroups.size() == 1);
			REQUIRE(patternMatches[0].thePattern == NRange(5, 6));
			REQUIRE(patternMatches[0].theGroups[0] == NRange(8, 3));

			REQUIRE(patternMatches[1].theGroups.size() == 1);
			REQUIRE(patternMatches[1].thePattern == NRange(15, 6));
			REQUIRE(patternMatches[1].theGroups[0] == NRange(18, 3));

			REQUIRE(patternMatches[2].theGroups.size() == 1);
			REQUIRE(patternMatches[2].thePattern == NRange(25, 6));
			REQUIRE(patternMatches[2].theGroups[0] == NRange(28, 3));

			REQUIRE(patternMatches[3].theGroups.size() == 1);
			REQUIRE(patternMatches[3].thePattern == NRange(37, 6));
			REQUIRE(patternMatches[3].theGroups[0] == NRange(40, 3));
		}


		patternMatches = theString.FindMatches("\\sWO(\\w+)", kNStringNoCase);
		if (theString == kTestStringSmall)
		{
			REQUIRE(patternMatches.size() == 1);
			REQUIRE(patternMatches[0].theGroups.size() == 1);
			REQUIRE(patternMatches[0].thePattern == NRange(5, 6));
			REQUIRE(patternMatches[0].theGroups[0] == NRange(8, 3));
		}
		else
		{
			REQUIRE(patternMatches.size() == 4);
			REQUIRE(patternMatches[0].theGroups.size() == 1);
			REQUIRE(patternMatches[0].thePattern == NRange(5, 6));
			REQUIRE(patternMatches[0].theGroups[0] == NRange(8, 3));

			REQUIRE(patternMatches[1].theGroups.size() == 1);
			REQUIRE(patternMatches[1].thePattern == NRange(15, 6));
			REQUIRE(patternMatches[1].theGroups[0] == NRange(18, 3));

			REQUIRE(patternMatches[2].theGroups.size() == 1);
			REQUIRE(patternMatches[2].thePattern == NRange(25, 6));
			REQUIRE(patternMatches[2].theGroups[0] == NRange(28, 3));

			REQUIRE(patternMatches[3].theGroups.size() == 1);
			REQUIRE(patternMatches[3].thePattern == NRange(37, 6));
			REQUIRE(patternMatches[3].theGroups[0] == NRange(40, 3));
		}


		patternMatches = theString.FindMatches("\\sWO(\\w+)");
		REQUIRE(patternMatches.empty());
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetMatch")
{


	// Perform the test
	NString theMatch;

	for (auto theString : stringObjects)
	{
		theMatch = theString.GetMatch("\\sWo\\w+");
		if (theString == kTestStringSmall)
		{
			REQUIRE(theMatch == " World");
		}
		else
		{
			REQUIRE(theMatch == " World");
		}


		theMatch = theString.GetMatch("\\sWo(\\w+)");
		if (theString == kTestStringSmall)
		{
			REQUIRE(theMatch == "rld");
		}
		else
		{
			REQUIRE(theMatch == "rld");
		}


		theMatch = theString.GetMatch("\\sWO(\\w+)", kNStringNoCase);
		if (theString == kTestStringSmall)
		{
			REQUIRE(theMatch == "rld");
		}
		else
		{
			REQUIRE(theMatch == "rld");
		}


		theMatch = theString.GetMatch("\\sWO(\\w+)");
		REQUIRE(theMatch.IsEmpty());
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetMatches")
{


	// Perform the test
	NVectorString theMatches;

	for (auto theString : stringObjects)
	{
		theMatches = theString.GetMatches("\\sWo\\w+");
		if (theString == kTestStringSmall)
		{
			REQUIRE(theMatches.size() == 1);
			REQUIRE(theMatches[0] == " World");
		}
		else
		{
			REQUIRE(theMatches.size() == 4);
			REQUIRE(theMatches[0] == " World");
			REQUIRE(theMatches[1] == " World");
			REQUIRE(theMatches[2] == " World");
			REQUIRE(theMatches[3] == " World");
		}


		theMatches = theString.GetMatches("\\sWo(\\w+)");
		if (theString == kTestStringSmall)
		{
			REQUIRE(theMatches.size() == 1);
			REQUIRE(theMatches[0] == "rld");
		}
		else
		{
			REQUIRE(theMatches.size() == 4);
			REQUIRE(theMatches[0] == "rld");
			REQUIRE(theMatches[1] == "rld");
			REQUIRE(theMatches[2] == "rld");
			REQUIRE(theMatches[3] == "rld");
		}


		theMatches = theString.GetMatches("\\sWO(\\w+)", kNStringNoCase);
		if (theString == kTestStringSmall)
		{
			REQUIRE(theMatches.size() == 1);
			REQUIRE(theMatches[0] == "rld");
		}
		else
		{
			REQUIRE(theMatches.size() == 4);
			REQUIRE(theMatches[0] == "rld");
			REQUIRE(theMatches[1] == "rld");
			REQUIRE(theMatches[2] == "rld");
			REQUIRE(theMatches[3] == "rld");
		}


		theMatches = theString.GetMatches("\\sWO(\\w+)");
		REQUIRE(theMatches.empty());
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Replace/String")
{


	// Perform the test
	NString theString;

	theString = kTestStringSmall;
	REQUIRE(theString.Replace("World", "You"));
	REQUIRE(theString == "Hello You");

	theString = kTestStringSmall;
	REQUIRE(theString.Replace("WORLD", "YOU", kNStringNoCase));
	REQUIRE(theString == "Hello YOU");

	theString = kTestStringSmall;
	REQUIRE(theString.Replace("Wo..d", "You", kNStringPattern));
	REQUIRE(theString == "Hello You");

	theString = kTestStringSmall;
	REQUIRE(theString.Replace("WO..D", "YOU", kNStringPatternNoCase));
	REQUIRE(theString == "Hello YOU");


	theString = kTestStringLarge;
	REQUIRE(theString.Replace("Three", "Your"));
	REQUIRE(theString == "Hello World One World Two World Your World");

	theString = kTestStringLarge;
	REQUIRE(theString.Replace("THREE", "YOUR", kNStringNoCase));
	REQUIRE(theString == "Hello World One World Two World YOUR World");

	theString = kTestStringLarge;
	REQUIRE(theString.Replace("Thre+", "Your", kNStringPattern));
	REQUIRE(theString == "Hello World One World Two World Your World");

	theString = kTestStringLarge;
	REQUIRE(theString.Replace("THR..", "YOUR", kNStringPatternNoCase));
	REQUIRE(theString == "Hello World One World Two World YOUR World");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "ReplaceAll/String")
{


	// Perform the test
	NString theString;

	theString = kTestStringSmall;
	REQUIRE(theString.ReplaceAll("o", "a") == 2);
	REQUIRE(theString == "Hella Warld");

	theString = kTestStringSmall;
	REQUIRE(theString.ReplaceAll("O", "A", kNStringNoCase) == 2);
	REQUIRE(theString == "HellA WArld");

	theString = kTestStringSmall;
	REQUIRE(theString.ReplaceAll("ll.", "pp", kNStringPattern) == 1);
	REQUIRE(theString == "Hepp World");

	theString = kTestStringSmall;
	REQUIRE(theString.ReplaceAll("LL.", "PP", kNStringPatternNoCase) == 1);
	REQUIRE(theString == "HePP World");


	theString = kTestStringLarge;
	REQUIRE(theString.ReplaceAll("World", "You") == 4);
	REQUIRE(theString == "Hello You One You Two You Three You");

	theString = kTestStringLarge;
	REQUIRE(theString.ReplaceAll("WORLD", "YOU", kNStringNoCase) == 4);
	REQUIRE(theString == "Hello YOU One YOU Two YOU Three YOU");

	theString = kTestStringLarge;
	REQUIRE(theString.ReplaceAll("Wo..d", "You", kNStringPattern) == 4);
	REQUIRE(theString == "Hello You One You Two You Three You");

	theString = kTestStringLarge;
	REQUIRE(theString.ReplaceAll("WO..D", "YOU", kNStringPatternNoCase) == 4);
	REQUIRE(theString == "Hello YOU One YOU Two YOU Three YOU");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Replace/Range")
{


	// Perform the test
	NString theString;

	theString = kTestStringSmall;
	theString.Replace(NRange(6, 5), "You");
	REQUIRE(theString == "Hello You");

	theString = kTestStringLarge;
	theString.Replace(NRange(6, 5), "You");
	REQUIRE(theString == "Hello You One World Two World Three World");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "ReplaceAll/Range")
{


	// Perform the test
	NString theString;

	theString = kTestStringSmall;
	theString.ReplaceAll({NRange(1, 3), NRange(5, 2), NRange(8, 3)}, "");
	REQUIRE(theString == "Hoo");

	theString = kTestStringSmall;
	theString.ReplaceAll({NRange(1, 3), NRange(5, 2), NRange(8, 33)}, "");
	REQUIRE(theString == "Hoo");

	theString = kTestStringSmall;
	theString.ReplaceAll({NRange(1, 3), NRange(5, 2), NRange(88, 33)}, "");
	REQUIRE(theString == "Hoorld");

	theString = kTestStringLarge;
	theString.ReplaceAll({NRange(11, 10), NRange(21, 10), NRange(31, 12)}, "");
	REQUIRE(theString == "Hello World");

	theString = kTestStringLarge;
	theString.ReplaceAll({NRange(31, 12), NRange(21, 10), NRange(11, 10)}, "");
	REQUIRE(theString == "Hello World");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetTransformed")
{


	// Perform the test
	NString tmpString;

	for (auto theString : stringObjects)
	{
		tmpString = theString.GetTransformed(kNStringTransformNone);
		REQUIRE(tmpString == theString);

		tmpString = theString.GetTransformed(kNStringTransformToLower);
		REQUIRE(tmpString != theString);

		tmpString = theString.GetTransformed(kNStringTransformToUpper);
		REQUIRE(tmpString != theString);
	}

	tmpString = NString("one two three").GetTransformed(kNStringTransformCapitalizeWords);
	REQUIRE(tmpString == "One Two Three");

	tmpString = NString("one? TWO! three.").GetTransformed(kNStringTransformCapitalizeSentences);
	REQUIRE(tmpString == "One? Two! Three.");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetLower")
{


	// Perform the test
	NString       tmpString;
	const utf8_t* textUTF8;

	for (auto theString : stringObjects)
	{
		tmpString = theString.GetLower();
		textUTF8  = tmpString.GetUTF8();

		while (*textUTF8 != 0x00)
		{
			REQUIRE((!isalpha(*textUTF8) || islower(*textUTF8)));
			textUTF8++;
		}
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetUpper")
{


	// Perform the test
	NString       tmpString;
	const utf8_t* textUTF8;

	for (auto theString : stringObjects)
	{
		tmpString = theString.GetUpper();
		textUTF8  = tmpString.GetUTF8();

		while (*textUTF8 != 0x00)
		{
			REQUIRE((!isalpha(*textUTF8) || isupper(*textUTF8)));
			textUTF8++;
		}
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "StartsWith")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.StartsWith("Hello"));
		REQUIRE(theString.StartsWith("HELLO", kNStringNoCase));

		REQUIRE(theString.StartsWith("Hel+o", kNStringPattern));
		REQUIRE(theString.StartsWith("HEL+O", kNStringPatternNoCase));
	}

	REQUIRE(NString(".aaa").StartsWith("."));
	REQUIRE(!NString("aaa").StartsWith("."));

	REQUIRE(NString(".aaa").StartsWith(".", kNStringPattern));
	REQUIRE(NString("aaaa").StartsWith(".", kNStringPattern));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "EndsWith")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.EndsWith("World"));
		REQUIRE(theString.EndsWith("WORLD", kNStringNoCase));

		REQUIRE(theString.EndsWith("Wo\\w\\wd", kNStringPattern));
		REQUIRE(theString.EndsWith("WO\\w\\wD", kNStringPatternNoCase));
	}

	REQUIRE(NString("aaa.").EndsWith("."));
	REQUIRE(!NString("aaa").EndsWith("."));

	REQUIRE(NString("aaa.").EndsWith(".", kNStringPattern));
	REQUIRE(NString("aaaa").EndsWith(".", kNStringPattern));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Contains")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.Contains("llo Wor"));
		REQUIRE(theString.Contains("LLO WOR", kNStringNoCase));

		REQUIRE(theString.Contains("l+o\\sW.r", kNStringPattern));
		REQUIRE(theString.Contains("L+O\\sW.R", kNStringPatternNoCase));
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Compare")
{


	// Perform the test
	NString stringA = "test";
	NString stringB = "TeSt";
	NString stringC;

	REQUIRE(stringA.Compare(stringB, kNStringNone) == NComparison::GreaterThan);
	REQUIRE(stringA.Compare(stringB, kNStringNoCase) == NComparison::EqualTo);

	REQUIRE(stringA.Compare(stringC, kNStringNone) == NComparison::GreaterThan);
	REQUIRE(stringA.Compare(stringC, kNStringNoCase) == NComparison::GreaterThan);


	REQUIRE(stringB.Compare(stringA, kNStringNone) == NComparison::LessThan);
	REQUIRE(stringB.Compare(stringA, kNStringNoCase) == NComparison::EqualTo);

	REQUIRE(stringB.Compare(stringC, kNStringNone) == NComparison::GreaterThan);
	REQUIRE(stringB.Compare(stringC, kNStringNoCase) == NComparison::GreaterThan);


	REQUIRE(stringC.Compare(stringA, kNStringNone) == NComparison::LessThan);
	REQUIRE(stringC.Compare(stringA, kNStringNoCase) == NComparison::LessThan);

	REQUIRE(stringC.Compare(stringB, kNStringNone) == NComparison::LessThan);
	REQUIRE(stringC.Compare(stringB, kNStringNoCase) == NComparison::LessThan);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "EqualTo")
{


	// Perform the test
	NString stringA = "test";
	NString stringB = "TeSt";

	REQUIRE(stringA.EqualTo(stringB));
	REQUIRE(!stringA.EqualTo(stringB, kNStringNone));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetPrefix")
{


	// Perform the test
	REQUIRE(kTestStringSmall.GetPrefix(0) == "");
	REQUIRE(kTestStringSmall.GetPrefix(3) == "Hel");
	REQUIRE(kTestStringSmall.GetPrefix(5) == "Hello");

	REQUIRE(kTestStringLarge.GetPrefix(0) == "");
	REQUIRE(kTestStringLarge.GetPrefix(3) == "Hel");
	REQUIRE(kTestStringLarge.GetPrefix(5) == "Hello");

	REQUIRE(kTestStringSmall.GetPrefix(kTestStringSmall.GetSize()) == kTestStringSmall);
	REQUIRE(kTestStringLarge.GetPrefix(kTestStringLarge.GetSize()) == kTestStringLarge);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetSuffix")
{


	// Perform the test
	REQUIRE(kTestStringSmall.GetSuffix(0) == "");
	REQUIRE(kTestStringSmall.GetSuffix(3) == "rld");
	REQUIRE(kTestStringSmall.GetSuffix(5) == "World");

	REQUIRE(kTestStringLarge.GetSuffix(0) == "");
	REQUIRE(kTestStringLarge.GetSuffix(3) == "rld");
	REQUIRE(kTestStringLarge.GetSuffix(5) == "World");

	REQUIRE(kTestStringSmall.GetSuffix(kTestStringSmall.GetSize()) == kTestStringSmall);
	REQUIRE(kTestStringLarge.GetSuffix(kTestStringLarge.GetSize()) == kTestStringLarge);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetSubstring")
{


	// Perform the test
	REQUIRE(kTestStringSmall.GetSubstring(kNRangeNone) == "");
	REQUIRE(kTestStringSmall.GetSubstring(kNRangeAll) == kTestStringSmall);

	REQUIRE(kTestStringSmall.GetSubstring(NRange(0, 5)) == "Hello");
	REQUIRE(kTestStringSmall.GetSubstring(NRange(2, 3)) == "llo");
	REQUIRE(kTestStringSmall.GetSubstring(NRange(6, 5)) == "World");

	REQUIRE(kTestStringLarge.GetSubstring(NRange(0, 5)) == "Hello");
	REQUIRE(kTestStringLarge.GetSubstring(NRange(22, 9)) == "Two World");
	REQUIRE(kTestStringLarge.GetSubstring(NRange(32, 11)) == "Three World");

	REQUIRE(kTestStringSmall.GetSubstring(NRange(0, kTestStringSmall.GetSize())) ==
			kTestStringSmall);
	REQUIRE(kTestStringLarge.GetSubstring(NRange(0, kTestStringLarge.GetSize())) ==
			kTestStringLarge);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetSubstrings")
{


	// Perform the test
	NVectorRange  theRanges;
	NVectorString theResult;

	theRanges = {{0, 5}, {2, 3}, {6, 5}};
	theResult = kTestStringSmall.GetSubstrings(theRanges);
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "Hello");
	REQUIRE(theResult[1] == "llo");
	REQUIRE(theResult[2] == "World");

	theRanges = {{0, 5}, {22, 9}, {32, 11}};
	theResult = kTestStringLarge.GetSubstrings(theRanges);
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "Hello");
	REQUIRE(theResult[1] == "Two World");
	REQUIRE(theResult[2] == "Three World");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "RemovePrefix")
{


	// Perform the test
	NString theString;

	theString = kTestStringSmall;
	theString.RemovePrefix(6);
	REQUIRE(theString == "World");

	theString = kTestStringSmall;
	theString.RemovePrefix(theString.GetSize());
	REQUIRE(theString == "");

	theString = kTestStringSmall;
	theString.RemovePrefix(6000);
	REQUIRE(theString == "");

	theString = kTestStringSmall;
	theString.RemovePrefix(0);
	REQUIRE(theString == kTestStringSmall);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "RemoveSuffix")
{


	// Perform the test
	NString theString;

	theString = kTestStringSmall;
	theString.RemoveSuffix(6);
	REQUIRE(theString == "Hello");

	theString = kTestStringSmall;
	theString.RemoveSuffix(theString.GetSize());
	REQUIRE(theString == "");

	theString = kTestStringSmall;
	theString.RemoveSuffix(6000);
	REQUIRE(theString == "");

	theString = kTestStringSmall;
	theString.RemoveSuffix(0);
	REQUIRE(theString == kTestStringSmall);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "RemoveSubstring")
{


	// Perform the test
	NString theString;

	theString = kTestStringSmall;
	theString.RemoveSubstring(NRange(3, 7));
	REQUIRE(theString == "Held");

	theString = kTestStringSmall;
	theString.RemoveSubstring(NRange(3, 6000));
	REQUIRE(theString == "Hel");

	theString = kTestStringSmall;
	theString.RemoveSubstring(NRange(3, 0));
	REQUIRE(theString == kTestStringSmall);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "RemoveSubstrings")
{


	// Perform the test
	NString theString;

	theString = kTestStringSmall;
	theString.RemoveSubstrings({NRange(1, 3), NRange(5, 2), NRange(8, 3)});
	REQUIRE(theString == "Hoo");

	theString = kTestStringSmall;
	theString.RemoveSubstrings({NRange(5, 2), NRange(1, 3), NRange(8, 33)});
	REQUIRE(theString == "Hoo");

	theString = kTestStringSmall;
	theString.RemoveSubstrings({NRange(1, 3), NRange(88, 33), NRange(5, 2)});
	REQUIRE(theString == "Hoorld");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "TrimPrefix")
{


	// Perform the test
	NString theString;

	theString = " \n \t Hello \t \n ";
	theString.TrimPrefix();
	REQUIRE(theString == "Hello \t \n ");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "TrimSuffix")
{


	// Perform the test
	NString theString;

	theString = " \n \t Hello \n \t ";
	theString.TrimSuffix();
	REQUIRE(theString == " \n \t Hello");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Trim")
{


	// Perform the test
	NString theString;

	theString = " \n \t Hello \n \t ";
	theString.Trim();
	REQUIRE(theString == "Hello");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Split")
{


	// Perform the test
	NString       kTokenComma(",");
	NVectorString theResult;

	theResult = NString("a").Split();
	REQUIRE(theResult.size() == 1);
	REQUIRE(theResult[0] == "a");

	theResult = NString("a b c").Split();
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");
	REQUIRE(theResult[2] == "c");

	theResult = NString("a  b   c").Split();
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");
	REQUIRE(theResult[2] == "c");

	theResult = NString("a \tb\t \tc").Split();
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");
	REQUIRE(theResult[2] == "c");


	theResult = NString("").Split(kTokenComma);
	REQUIRE(theResult.size() == 1);
	REQUIRE(theResult[0] == "");

	theResult = NString(",").Split(kTokenComma);
	REQUIRE(theResult.size() == 2);
	REQUIRE(theResult[0] == "");
	REQUIRE(theResult[1] == "");

	theResult = NString(",,").Split(kTokenComma);
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "");
	REQUIRE(theResult[1] == "");
	REQUIRE(theResult[2] == "");


	theResult = NString("a,b,c").Split(kTokenComma);
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");
	REQUIRE(theResult[2] == "c");

	theResult = NString(",a,b,c").Split(kTokenComma);
	REQUIRE(theResult.size() == 4);
	REQUIRE(theResult[0] == "");
	REQUIRE(theResult[1] == "a");
	REQUIRE(theResult[2] == "b");
	REQUIRE(theResult[3] == "c");

	theResult = NString(",a,b,c,").Split(kTokenComma);
	REQUIRE(theResult.size() == 5);
	REQUIRE(theResult[0] == "");
	REQUIRE(theResult[1] == "a");
	REQUIRE(theResult[2] == "b");
	REQUIRE(theResult[3] == "c");
	REQUIRE(theResult[4] == "");

	theResult = NString(",a,,b,c,").Split(kTokenComma);
	REQUIRE(theResult.size() == 6);
	REQUIRE(theResult[0] == "");
	REQUIRE(theResult[1] == "a");
	REQUIRE(theResult[2] == "");
	REQUIRE(theResult[3] == "b");
	REQUIRE(theResult[4] == "c");
	REQUIRE(theResult[5] == "");

	theResult = NString("a,,,b,c").Split(kTokenComma);
	REQUIRE(theResult.size() == 5);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "");
	REQUIRE(theResult[2] == "");
	REQUIRE(theResult[3] == "b");
	REQUIRE(theResult[4] == "c");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Split/UTF")
{


	// Perform the test
	//
	// PCRE works with UTF8 strings internally so test that different
	// external encodings are transcoded internally as needed.
	//
	// Ideally we want to avoid transcoding unless absolutely necessary,
	// and share as much internal state amongst strings as we can when we
	// do, so test both small and large strings in three encodings.
	NVectorString theResult;

	theResult = NString(u8"a,b").Split(",");
	REQUIRE(theResult.size() == 2);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");

	theResult = NString(u8"aaaaaaaaaaaaaaaaaaaa,bbbbbbbbbbbbbbbbbbbb").Split(",");
	REQUIRE(theResult.size() == 2);
	REQUIRE(theResult[0] == "aaaaaaaaaaaaaaaaaaaa");
	REQUIRE(theResult[1] == "bbbbbbbbbbbbbbbbbbbb");


	theResult = NString(u"a,b").Split(",");
	REQUIRE(theResult.size() == 2);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");

	theResult = NString(u"aaaaaaaaaaaaaaaaaaaa,bbbbbbbbbbbbbbbbbbbb").Split(",");
	REQUIRE(theResult.size() == 2);
	REQUIRE(theResult[0] == "aaaaaaaaaaaaaaaaaaaa");
	REQUIRE(theResult[1] == "bbbbbbbbbbbbbbbbbbbb");


	theResult = NString(U"a,b").Split(",");
	REQUIRE(theResult.size() == 2);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");

	theResult = NString(U"aaaaaaaaaaaaaaaaaaaa,bbbbbbbbbbbbbbbbbbbb").Split(",");
	REQUIRE(theResult.size() == 2);
	REQUIRE(theResult[0] == "aaaaaaaaaaaaaaaaaaaa");
	REQUIRE(theResult[1] == "bbbbbbbbbbbbbbbbbbbb");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Split/WhitespacePattern")
{


	// Perform the test
	//
	// The default split separator is a pattern but, as most separators
	// are literals, the flags default to a literal.
	//
	// Splitting on whitespace will activate a pattern match automatically.
	//
	// Splitting on something that could be interpreted as a pattern, but
	// should be a literal, will be split as a literal.
	NVectorString theResult;

	theResult = NString("a  b").Split();
	REQUIRE(theResult.size() == 2);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");

	theResult = NString("a\\\\b").Split("\\");
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "");
	REQUIRE(theResult[2] == "b");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetLines")
{


	// Perform the test
	NVectorString theResult = kTestStringLines.GetLines();
	REQUIRE(theResult.size() == 7);
	REQUIRE(theResult[0] == "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
	REQUIRE(theResult[1] == "Vivamus est erat, scelerisque quis molestie a, bibendum eget risus.");
	REQUIRE(theResult[2] == "Etiam laoreet velit dolor, at eleifend enim ultrices vitae.");
	REQUIRE(theResult[3] == "");
	REQUIRE(theResult[4] == "");
	REQUIRE(theResult[5] == "");
	REQUIRE(theResult[6] == "Vivamus scelerisque sapien scelerisque ligula.");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "ResolveSlice")
{


	// Perform the test
	NString     theString = kTestStringLarge.GetSubstring(NRange(22, 9));
	const char* textUTF8  = theString.GetUTF8();

	REQUIRE(strlen(textUTF8) == 9);
	REQUIRE(strcmp(textUTF8, "Two World") == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Appendable")
{


	// Perform the test
	size_t oldSize = 0;

	for (auto theString : stringObjects)
	{
		oldSize = theString.GetSize();
		theString += kTestStringSmall;
		REQUIRE(theString.GetSize() == (oldSize + kTestStringSmall.GetSize()));

		oldSize = theString.GetSize();
		theString += kTestStringLarge;
		REQUIRE(theString.GetSize() == (oldSize + kTestStringLarge.GetSize()));

		theString = kTestStringSmall + kTestStringSmall;
		REQUIRE(theString.GetSize() == (kTestStringSmall.GetSize() * 2));

		theString = kTestStringLarge + kTestStringLarge;
		REQUIRE(theString.GetSize() == (kTestStringLarge.GetSize() * 2));
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Append/LargeUTF16SmallUTF8")
{


	// Perform the test
	//
	// Appending large + small may require transcoding.
	NString theString;

	theString = kTestStringLarge.GetUTF16();
	theString += kTestStringSmall.GetUTF8();

	REQUIRE(theString == (kTestStringLarge + kTestStringSmall));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Append/MultipleEncodings")
{


	// Perform the test
	//
	// Appending to a string with multiple encodings must release
	// the alternative encodings after appending new content.
	//
	// This can be tested by asking for one of the existing encodings,
	// and ensuring it contains the results of the append operation.
	NString theString;

	theString = u"Apple";           // u"Apple"				UTF16
	(void) theString.GetUTF8();     // u8"Apple"			UTF8
	(void) theString.GetUTF32();    // U"Apple"				UTF8

	theString += u8"Banana";    // u"AppleBanana"		UTF16

	REQUIRE(strstr(theString.GetUTF8(), "Banana") != nullptr);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Append/MultipleEncodingsSliced")
{


	// Perform the test
	//
	// Splitting a string returns slices, and appending to a string
	// maintains the encoding of the string being appended to.
	//
	// This can require that individual slices of the split string
	// be transcoded in order to be appended to the result.
	NString pathInput(u"/path/as/utf16/assigned/to/utf8");
	NString pathOutput(u8"");
	NString pathDelim("/");

	for (const auto& thePart : pathInput.Split(pathDelim))
	{
		if (!thePart.IsEmpty())
		{
			pathOutput += pathDelim;
			pathOutput += thePart;
		}
	}

	REQUIRE(pathOutput == pathInput);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "CompareEqual")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		if (theString == kTestStringSmall)
		{
			REQUIRE(theString != kTestStringLarge);
		}
		else if (theString == kTestStringLarge)
		{
			REQUIRE(theString != kTestStringSmall);
		}
	}

	REQUIRE(NString("1234") != NString("abcd"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "CompareOrder")
{


	// Perform the test
	REQUIRE(kTestStringSmall <= kTestStringSmall);
	REQUIRE(kTestStringSmall <= kTestStringLarge);
	REQUIRE(kTestStringSmall < kTestStringLarge);

	REQUIRE(kTestStringLarge >= kTestStringLarge);
	REQUIRE(kTestStringLarge >= kTestStringSmall);
	REQUIRE(kTestStringLarge > kTestStringSmall);

	REQUIRE(NString("A") < NString("B"));
	REQUIRE(NString("B") > NString("A"));

	REQUIRE(NString("A") < NString("a"));
	REQUIRE(NString("a") > NString("A"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Hashable")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.GetHash() != 0);
	}


	NString stringUTF8("test");
	NString stringUTF16(u"test");
	NString stringUTF32(U"test");

	REQUIRE(stringUTF8.GetHash() == stringUTF16.GetHash());
	REQUIRE(stringUTF8.GetHash() == stringUTF32.GetHash());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", kTestStringSmall) == "Hello World");
}
