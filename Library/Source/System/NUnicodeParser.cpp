/*	NAME:
		NUnicodeParser.cpp

	DESCRIPTION:
		Unicode parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <ctype.h>

#include "NStringEncoder.h"
#include "NUnicodeParser.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// UTF16
static const UTF16Char kUTF16SurrogateHighStart						= 0xD800;
static const UTF16Char kUTF16SurrogateHighEnd						= 0xDBFF;
static const UTF16Char kUTF16SurrogateLowStart						= 0xDC00;
static const UTF16Char kUTF16SurrogateLowEnd						= 0xDFFF;


// UTF8
static const NIndex kUTF8TrailingBytes[256] =					{	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
																	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
																	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
																	3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5 };





//============================================================================
//		NUnicodeParser::NUnicodeParser : Constructor.
//----------------------------------------------------------------------------
NUnicodeParser::NUnicodeParser(const NData &theData, NStringEncoding theEncoding)
{


	// Initialise ourselves
	mData     = theData;
	mEncoding = theEncoding;
	
	switch (mEncoding) {
		case kNStringEncodingUTF8:
			mCodePoints = GetCodePointsUTF8();
			break;

		case kNStringEncodingUTF16:
			mCodePoints = GetCodePointsUTF16();
			break;

		case kNStringEncodingUTF32:
			mCodePoints = GetCodePointsUTF32();
			break;
		
		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			break;
		}
}





//============================================================================
//		NUnicodeParser::~NUnicodeParser : Destructor.
//----------------------------------------------------------------------------
NUnicodeParser::~NUnicodeParser(void)
{
}





//============================================================================
//		NUnicodeParser::GetSize : Get the size.
//----------------------------------------------------------------------------
NIndex NUnicodeParser::GetSize(void) const
{


	// Get the size
	return(mCodePoints.size());
}





//============================================================================
//		NUnicodeParser::GetRange : Get the range of a character.
//----------------------------------------------------------------------------
NRange NUnicodeParser::GetRange(NIndex theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex >= 0 && theIndex < GetSize());
	
	
	
	// Get the range
	return(mCodePoints[theIndex]);
}





//============================================================================
//		NUnicodeParser::GetChar : Get a character.
//----------------------------------------------------------------------------
UTF32Char NUnicodeParser::GetChar(NIndex theIndex) const
{	NData				srcData, dstData;
	NStringEncoder		theEncoder;
	UTF32Char			theResult;
	NRange				theRange;
	NStatus				theErr;



	// Validate our parameters
	NN_ASSERT(theIndex >= 0 && theIndex < GetSize());



	// Get the state we need
	theRange = GetRange(theIndex);
	srcData  = NData(theRange.GetSize(), mData.GetData(theRange.GetLocation()));



	// Convert the character
	theResult = 0;
	theErr    = theEncoder.Convert(srcData, dstData, mEncoding, kNStringEncodingUTF32);

	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(dstData.GetSize() == sizeof(theResult));



	// Get the result
	if (theErr == noErr && dstData.GetSize() == sizeof(theResult))
		theResult = *((UTF32Char *) dstData.GetData());
	
	return(theResult);
}





//============================================================================
//		NUnicodeParser::IsSpace : Is this a whitespace character?
//----------------------------------------------------------------------------
bool NUnicodeParser::IsSpace(UTF32Char theChar) const
{	bool	theResult;



	// Check the character
	//
	// For now we use a simple ASCII test.
	theResult = (theChar <= 0xFF && isspace(theChar));
	
	return(theResult);
}





//============================================================================
//		NUnicodeParser::IsPunct : Is this a punctuation character?
//----------------------------------------------------------------------------
bool NUnicodeParser::IsPunct(UTF32Char theChar) const
{	bool	theResult;



	// Check the character
	//
	// For now we use a simple ASCII test.
	theResult = (theChar <= 0xFF && ispunct(theChar));
	
	return(theResult);
}





//============================================================================
//		NUnicodeParser::IsAlpha : Is this an alphabetic character?
//----------------------------------------------------------------------------
bool NUnicodeParser::IsAlpha(UTF32Char theChar) const
{	bool	theResult;



	// Check the character
	//
	// For now we use a simple ASCII test.
	theResult = (theChar <= 0xFF && isalpha(theChar));
	
	return(theResult);
}





//============================================================================
//		NUnicodeParser::IsDigit : Is this a numeric character?
//----------------------------------------------------------------------------
bool NUnicodeParser::IsDigit(UTF32Char theChar) const
{	bool	theResult;



	// Check the character
	//
	// For now we use a simple ASCII test.
	theResult = (theChar <= 0xFF && isdigit(theChar));
	
	return(theResult);
}





//============================================================================
//		NUnicodeParser::GetLower : Get the lower-case version of a character.
//----------------------------------------------------------------------------
UTF32Char NUnicodeParser::GetLower(UTF32Char theChar) const
{


	// Convert the character
	//
	// For now we use a simple ASCII test.
	if (theChar <= 0xFF)
		theChar = tolower(theChar);
	
	return(theChar);
}





//============================================================================
//		NUnicodeParser::GetUpper : Get the upper-case version of a character.
//----------------------------------------------------------------------------
UTF32Char NUnicodeParser::GetUpper(UTF32Char theChar) const
{


	// Convert the character
	//
	// For now we use a simple ASCII test.
	if (theChar <= 0xFF)
		theChar = toupper(theChar);
	
	return(theChar);
}





//============================================================================
//		NUnicodeParser::GetCodePointsUTF8 : Get the code points from a UTF8 string.
//----------------------------------------------------------------------------
#pragma mark -
NRangeList NUnicodeParser::GetCodePointsUTF8(void) const
{	NIndex				n, theSize, charSize;
	NRangeList			theResult;
	NRange				theRange;
	const UTF8Char		*theData;



	// Get the state we need
	theSize =                    mData.GetSize();
	theData = (const UTF8Char *) mData.GetData();



	// Get the code points
	n = 0;

	while (n < theSize)
		{
		NN_ASSERT(n >= 0 && n < (NIndex) sizeof(kUTF8TrailingBytes));
		charSize = kUTF8TrailingBytes[n] + 1;

		theRange = NRange(n, charSize);
		theResult.push_back(theRange);
		
		n += charSize;
		}

	return(theResult);
}





//============================================================================
//		NUnicodeParser::GetCodePointsUTF16 : Get the code points from a UTF16 string.
//----------------------------------------------------------------------------
NRangeList NUnicodeParser::GetCodePointsUTF16(void) const
{	NIndex				n, theSize, charSize;
	NRangeList			theResult;
	NRange				theRange;
	const UTF16Char		*theData;
	UTF16Char			theChar;



	// Get the state we need
	theSize =                  mData.GetSize();
	theData = (const UInt16 *) mData.GetData();

	NN_ASSERT((theSize % sizeof(UTF16Char)) == 0);



	// Get the code points
	n = 0;

	while (n < theSize)
		{
		theChar  = theData[n];
		charSize = 2;
		
		if (theChar >= kUTF16SurrogateHighStart && theChar <= kUTF16SurrogateHighEnd)
			{
			NN_ASSERT(n <= (theSize-2));
			NN_ASSERT(theData[n+2] >= kUTF16SurrogateLowStart && theData[n+2] <= kUTF16SurrogateLowEnd);
			
			charSize += 2;
			}

		theRange = NRange(n, charSize);
		theResult.push_back(theRange);
		
		n += charSize;
		}

	return(theResult);
}





//============================================================================
//		NUnicodeParser::GetCodePointsUTF32 : Get the code points from a UTF32 string.
//----------------------------------------------------------------------------
NRangeList NUnicodeParser::GetCodePointsUTF32(void) const
{	NIndex				n, theSize;
	NRangeList			theResult;
	NRange				theRange;



	// Get the state we need
	theSize = mData.GetSize();
	
	NN_ASSERT((theSize % sizeof(UTF32Char)) == 0);



	// Get the code points
	for (n = 0; n < theSize; n += 4)
		{
		theRange = NRange(n, 4);
		theResult.push_back(theRange);
		}

	return(theResult);
}


