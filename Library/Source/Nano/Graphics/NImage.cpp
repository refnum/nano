/*	NAME:
		NImage.cpp

	DESCRIPTION:
		Image buffer.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetSystem.h"
#include "NTargetMath.h"
#include "NImage.h"





//============================================================================
//		NImage::NImage : Constructor.
//----------------------------------------------------------------------------
NImage::NImage(const NData &theData)
{	NStatus		theErr;



	// Initialize ourselves
	Clear();



	// Decode the image
	theErr = Decode(theData);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NImage::NImage : Constructor.
//----------------------------------------------------------------------------
NImage::NImage(const NFile &theFile)
{	NStatus		theErr;



	// Initialize ourselves
	Clear();



	// Load the image
	theErr = Load(theFile);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NImage::NImage : Constructor.
//----------------------------------------------------------------------------
NImage::NImage(const NSize &theSize, NImageFormat theFormat, const NData &theData, NIndex rowBytes)
{	NIndex		packedRow;



	// Validate our parameters
	NN_ASSERT(!theSize.IsEmpty());



	// Initialize ourselves
	Clear();

	mSize   = theSize;
	mFormat = theFormat;



	// Create the image
	packedRow = (NIndex) (mSize.width * GetBytesPerPixel());

	if (theData.IsEmpty())
		{
		mData.SetSize((NIndex) (mSize.height * packedRow));
		mRowBytes = packedRow;
		}
	else
		{
		mData     = theData;
		mRowBytes = (rowBytes != 0) ? rowBytes : packedRow;
		}

	NN_ASSERT(!mData.IsEmpty());
}





//============================================================================
//		NImage::NImage : Constructor.
//----------------------------------------------------------------------------
NImage::NImage(void)
{


	// Initialize ourselves
	Clear();
}





//============================================================================
//		NImage::~NImage : Destructor.
//----------------------------------------------------------------------------
NImage::~NImage(void)
{
}





//============================================================================
//		NImage::IsValid : Is the image valid?
//----------------------------------------------------------------------------
bool NImage::IsValid(void) const
{


	// Check our state
	return(mFormat != kNImageFormatNone);
}





//============================================================================
//		NImage::Clear : Clear the image.
//----------------------------------------------------------------------------
void NImage::Clear(void)
{


	// Reset our state
	mSize.Clear();
	mData.Clear();

	mFormat   = kNImageFormatNone;
	mRowBytes = 0;
}





//============================================================================
//		NImage::ForEach : Process each pixel.
//----------------------------------------------------------------------------
void NImage::ForEach(const NImageForEachImmutableFunctor &theFunctor) const
{	NIndex			x, y, theWidth, theHeight, rowBytes, pixelBytes;
	const UInt8		*basePtr, *pixelPtr;



	// Get the state we need
	theWidth   = GetWidth();
	theHeight  = GetHeight();
	rowBytes   = GetBytesPerRow();
	pixelBytes = GetBytesPerPixel();
	basePtr    = GetPixels();



	// Process the pixels
	for (y = 0; y < theHeight; y++)
		{
		pixelPtr = basePtr + (y * rowBytes);

		for (x = 0; x < theWidth; x++)
			{
			if (!theFunctor(x, y, pixelPtr))
				return;
			
			pixelPtr += pixelBytes;
			}
		}
}





//============================================================================
//		NImage::ForEach : Process each pixel.
//----------------------------------------------------------------------------
void NImage::ForEach(const NImageForEachMutableFunctor &theFunctor)
{	NIndex		x, y, theWidth, theHeight, rowBytes, pixelBytes;
	UInt8		*basePtr, *pixelPtr;



	// Get the state we need
	theWidth   = GetWidth();
	theHeight  = GetHeight();
	rowBytes   = GetBytesPerRow();
	pixelBytes = GetBytesPerPixel();
	basePtr    = GetPixels();



	// Process the pixels
	for (y = 0; y < theHeight; y++)
		{
		pixelPtr = basePtr + (y * rowBytes);

		for (x = 0; x < theWidth; x++)
			{
			if (!theFunctor(x, y, pixelPtr))
				return;
			
			pixelPtr += pixelBytes;
			}
		}
}





//============================================================================
//		NImage::GetWidth : Get the width.
//----------------------------------------------------------------------------
NIndex NImage::GetWidth(void) const
{


	// Get the width
	return((NIndex) mSize.width);
}





//============================================================================
//		NImage::GetHeight : Get the height.
//----------------------------------------------------------------------------
NIndex NImage::GetHeight(void) const
{


	// Get the height
	return((NIndex) mSize.height);
}





//============================================================================
//		NImage::GetSize : Get the size.
//----------------------------------------------------------------------------
NSize NImage::GetSize(void) const
{


	// Get the size
	return(mSize);
}





//============================================================================
//		NImage::GetBounds : Get the bounds.
//----------------------------------------------------------------------------
NRectangle NImage::GetBounds(void) const
{


	// Get the bounds
	return(NRectangle(mSize));
}





//============================================================================
//		NImage::GetFormat : Get the format.
//----------------------------------------------------------------------------
NImageFormat NImage::GetFormat(void) const
{


	// Get the format
	return(mFormat);
}





//============================================================================
//		NImage::SetFormat : Set the format.
//----------------------------------------------------------------------------
void NImage::SetFormat(NImageFormat theFormat)
{	bool	didSet;



	// Process the image
	didSet = true;

	switch (mFormat) {
		case kNImageFormat_RGBX_8888:
		case kNImageFormat_RGBA_8888:
			switch (theFormat) {
				case kNImageFormat_RGBX_8888:
				case kNImageFormat_RGBA_8888:
					// No-op
					break;

				case kNImageFormat_XRGB_8888:
				case kNImageFormat_ARGB_8888:
					ForEach(BindSelf(NImage::PixelRotate32, _3, 8));
					break;

				case kNImageFormat_BGRX_8888:
				case kNImageFormat_BGRA_8888:
					ForEach(BindSelf(NImage::PixelSwizzle32, _3, mkvector(2, 1, 0, 3)));
					break;

				default:
					didSet = false;
					break;
				}
			break;


		case kNImageFormat_XRGB_8888:
		case kNImageFormat_ARGB_8888:
			switch (theFormat) {
				case kNImageFormat_RGBX_8888:
				case kNImageFormat_RGBA_8888:
					ForEach(BindSelf(NImage::PixelRotate32, _3, 24));
					break;

				case kNImageFormat_XRGB_8888:
				case kNImageFormat_ARGB_8888:
					// No-op
					break;

				case kNImageFormat_BGRX_8888:
				case kNImageFormat_BGRA_8888:
					ForEach(BindSelf(NImage::PixelSwizzle32, _3, mkvector(3, 2, 1, 0)));
					break;

				default:
					didSet = false;
					break;
				}
			break;


		case kNImageFormat_BGRX_8888:
		case kNImageFormat_BGRA_8888:
			switch (theFormat) {
				case kNImageFormat_RGBX_8888:
				case kNImageFormat_RGBA_8888:
					ForEach(BindSelf(NImage::PixelSwizzle32, _3, mkvector(2, 1, 0, 3)));
					break;

				case kNImageFormat_XRGB_8888:
				case kNImageFormat_ARGB_8888:
					ForEach(BindSelf(NImage::PixelSwizzle32, _3, mkvector(3, 2, 1, 0)));
					break;

				case kNImageFormat_BGRX_8888:
				case kNImageFormat_BGRA_8888:
					// No-op
					break;

				default:
					didSet = false;
					break;
				}
			break;


		default:
			didSet = false;
			break;
		}



	// Update our state
	if (didSet)
		mFormat = theFormat;
	else
		NN_LOG("Unable to convert image from format %ld to format %ld!", mFormat, theFormat);
}





//============================================================================
//		NImage::GetBitsPerPixel : Get the bits-per-pixel.
//----------------------------------------------------------------------------
NIndex NImage::GetBitsPerPixel(void) const
{	NIndex		theValue;



	// Get the value
	switch (mFormat) {
		case kNImageFormat_RGBX_8888:
		case kNImageFormat_RGBA_8888:
		case kNImageFormat_XRGB_8888:
		case kNImageFormat_ARGB_8888:
		case kNImageFormat_BGRX_8888:
		case kNImageFormat_BGRA_8888:
			theValue = 32;
			break;
		
		default:
			NN_LOG("Invalid image format: %ld", mFormat);
			theValue = 0;
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NImage::GetBitsPerComponent : Get the bits-per-component.
//----------------------------------------------------------------------------
NIndex NImage::GetBitsPerComponent(void) const
{	NIndex		theValue;



	// Get the value
	switch (mFormat) {
		case kNImageFormat_RGBX_8888:
		case kNImageFormat_RGBA_8888:
		case kNImageFormat_XRGB_8888:
		case kNImageFormat_ARGB_8888:
		case kNImageFormat_BGRX_8888:
		case kNImageFormat_BGRA_8888:
			theValue = 8;
			break;
		
		default:
			NN_LOG("Invalid image format: %ld", mFormat);
			theValue = 0;
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NImage::GetBytesPerPixel : Get the bytes-per-pixel.
//----------------------------------------------------------------------------
NIndex NImage::GetBytesPerPixel(void) const
{	NIndex	theValue;



	// Get the value
	theValue = (GetBitsPerPixel() / 8);
	NN_ASSERT((theValue * 8) == GetBitsPerPixel());
	
	return(theValue);
}





//============================================================================
//		NImage::GetBytesPerRow : Get the bytes-per-row.
//----------------------------------------------------------------------------
NIndex NImage::GetBytesPerRow(void) const
{


	// Get the value
	return(mRowBytes);
}





//============================================================================
//		NImage::GetPixels : Get the pixels.
//----------------------------------------------------------------------------
const UInt8 *NImage::GetPixels(NIndex x, NIndex y) const
{	NIndex			theOffset;
	const UInt8		*thePtr;



	// Validate our parameters
	NN_ASSERT(x >= 0 && x <= (NIndex) mSize.width);
	NN_ASSERT(y >= 0 && y <= (NIndex) mSize.height);



	// Get the pixels
	theOffset = (y * GetBytesPerRow()) + (x * GetBytesPerPixel());
	thePtr    = mData.GetData(theOffset);

	return(thePtr);
}





//============================================================================
//		NImage::GetPixels : Get the pixels.
//----------------------------------------------------------------------------
UInt8 *NImage::GetPixels(NIndex x, NIndex y)
{	NIndex		theOffset;
	UInt8		*thePtr;



	// Validate our parameters
	NN_ASSERT(x >= 0 && x <= (NIndex) mSize.width);
	NN_ASSERT(y >= 0 && y <= (NIndex) mSize.height);



	// Get the pixels
	theOffset = (y * GetBytesPerRow()) + (x * GetBytesPerPixel());
	thePtr    = mData.GetData(theOffset);

	return(thePtr);
}





//============================================================================
//		NImage::GetData : Get the data.
//----------------------------------------------------------------------------
NData NImage::GetData(void) const
{


	// Get the data
	return(mData);
}





//============================================================================
//		NImage::Load : Load an image.
//----------------------------------------------------------------------------
NStatus NImage::Load(const NFile &theFile, NImageFormat theFormat)
{	NData		theData;
	NStatus		theErr;



	// Load the image
	theData = NFileUtilities::GetFileData(theFile);
	theErr  = Decode(theData, theFormat);
	
	return(theErr);
}





//============================================================================
//		NImage::Save : Save an image.
//----------------------------------------------------------------------------
NStatus NImage::Save(const NFile &theFile, const NUTI &theType) const
{	NUTI		imageType;
	NData		theData;
	NStatus		theErr;



	// Get the state we need
	imageType = theType;

	if (!imageType.IsValid())
		imageType = theFile.GetUTI();

	if (!imageType.IsValid())
		imageType = kNUTTypePNG;



	// Save the image
	theData = Encode(imageType);
	theErr  = kNErrExhaustedSrc;

	if (!theData.IsEmpty())
		theErr = NFileUtilities::SetFileData(theFile, theData);
	
	return(theErr);
}





//============================================================================
//		NImage::Encode : Encode an image.
//----------------------------------------------------------------------------
NData NImage::Encode(const NUTI &theType) const
{


	// Encode the image
	return(NTargetSystem::ImageEncode(*this, theType));
}





//============================================================================
//		NImage::Decode : Decode an image.
//----------------------------------------------------------------------------
NStatus NImage::Decode(const NData &theData, NImageFormat theFormat)
{	NImage		theImage;



	// Decode the image
	theImage = NTargetSystem::ImageDecode(theData);
	if (!theImage.IsValid())
		return(kNErrMalformed);



	// Update our state
	*this = theImage;

	if (theFormat != kNImageFormatNone)
		SetFormat(theFormat);
	
	return(kNoErr);
}





//============================================================================
//		NImage::PixelRotate32 : Rotate a 32-bpp pixel.
//----------------------------------------------------------------------------
bool NImage::PixelRotate32(UInt8 *pixelPtr, UInt32 rotateRight)
{	UInt32	*pixelPtr32 = (UInt32 *) pixelPtr;



	// Validate our parameters
	NN_ASSERT(rotateRight < 32);
	NN_ASSERT((rotateRight % 8) == 0);



	// Rotate the pixel
	*pixelPtr32 = NTargetMath::RotateRight(*pixelPtr32, rotateRight);

	return(true);
}





//============================================================================
//		NImage::PixelSwizzle32 : Swizzle a 32-bpp pixel.
//----------------------------------------------------------------------------
bool NImage::PixelSwizzle32(UInt8 *pixelPtr, const NIndexList &newOrder)
{	UInt8	theBytes[4];



	// Validate our parameters
	NN_ASSERT(newOrder.size() == 4);



	// Swizzle the pixel
	*((UInt32 *) &theBytes[0]) = *((UInt32 *) pixelPtr);

	pixelPtr[0] = theBytes[newOrder[0]];
	pixelPtr[1] = theBytes[newOrder[1]];
	pixelPtr[2] = theBytes[newOrder[2]];
	pixelPtr[3] = theBytes[newOrder[3]];

	return(true);
}

