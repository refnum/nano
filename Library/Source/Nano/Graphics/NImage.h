/*	NAME:
		NImage.h

	DESCRIPTION:
		Image buffer.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NIMAGE_HDR
#define NIMAGE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NData.h"
#include "NUTI.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Image formats
//
// Each format defines the size, and order, of components within an image.
typedef enum {
	kNImageFormatNone,

	kNImageFormat_RGBX_8888,
	kNImageFormat_RGBA_8888,

	kNImageFormat_XRGB_8888,
	kNImageFormat_ARGB_8888,

	kNImageFormat_BGRX_8888,
	kNImageFormat_BGRA_8888
} NImageFormat;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Functors
typedef nfunctor<bool (NIndex x, NIndex y, const UInt8 *pixelPtr)>		NImageForEachImmutableFunctor;
typedef nfunctor<bool (NIndex x, NIndex y,       UInt8 *pixelPtr)>		NImageForEachMutableFunctor;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NImage {
public:
										NImage(const NData &theData);
										NImage(const NFile &theFile);

										NImage(	const NSize	   &theSize,
												NImageFormat	theFormat = kNImageFormat_RGBA_8888,
												const NData	   &theData   = NData(),
												NIndex			rowBytes  = 0);

										NImage(void);
	virtual							   ~NImage(void);


	// Is the image valid?
	bool								IsValid(void) const;


	// Clear the image
	void								Clear(void);


	// Process each pixel
	void								ForEach(const NImageForEachImmutableFunctor &theFunctor) const;
	void								ForEach(const NImageForEachMutableFunctor   &theFunctor);


	// Get the dimensions
	NIndex								GetWidth( void) const;
	NIndex								GetHeight(void) const;

	NSize								GetSize(  void) const;
	NRectangle							GetBounds(void) const;


	// Get/set the format
	NImageFormat						GetFormat(void) const;
	void								SetFormat(NImageFormat theFormat);


	// Get the structure
	NIndex								GetBitsPerPixel(    void) const;
	NIndex								GetBitsPerComponent(void) const;
	NIndex								GetBytesPerPixel(   void) const;
	NIndex								GetBytesPerRow(     void) const;


	// Get the pixels
	//
	// Const access is preferred, to avoid duplicating the underlying image data.
	const UInt8						   *GetPixels(NIndex x=0, NIndex y=0) const;
	UInt8							   *GetPixels(NIndex x=0, NIndex y=0);


	// Get the data
	NData								GetData(void) const;


	// Load/save the image
	//
	// Images are loaded in their existing format, and can be converted
	// to a specific format after loading.
	//
	// Images are saved to the UTI of their file ("foo.jpg" will produce
	// a JPEG), and can also be saved to a specific type.
	NStatus								Load(const NFile &theFile, NImageFormat	theFormat=kNImageFormatNone);
	NStatus								Save(const NFile &theFile, const NUTI &theType=NUTI()) const;


	// Encode/decode the image
	//
	// Images are decoded to their existing format, and can be converted
	// to a specific format after decoding.
	NData								Encode(const NUTI  &theType=kNUTTypePNG) const;
	NStatus								Decode(const NData &theData, NImageFormat theFormat=kNImageFormatNone);


private:
	bool								PixelRotate32( UInt8 *pixelPtr, UInt32 rotateRight);
	bool								PixelSwizzle32(UInt8 *pixelPtr, const NIndexList &newOrder);


private:
	NSize								mSize;
	NData								mData;
	NImageFormat						mFormat;
	NIndex								mRowBytes;
};







#endif // NCOLOR_HDR


