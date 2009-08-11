/*	NAME:
		NRectangle.h

	DESCRIPTION:
		Rectangle object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NRECTANGLE_HDR
#define NRECTANGLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"
#include "NPoint.h"
#include "NSize.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
class NRectangle;

extern const NRectangle kNRectangleZero;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NRectangle : public NComparable<NRectangle> {
public:
										 NRectangle(const Float32 x, Float32 y, Float32 width, Float32 height);
										 NRectangle(const NPoint &origin = kNPointZero,
													const NSize  &size   = kNSizeZero);
	virtual								~NRectangle(void);


	// Clear the rectangle
	void								Clear(void);


	// Test the rectangle
	bool								IsEmpty(void) const;


	// Compare the value
	NComparison							Compare(const NRectangle &theValue) const;


public:
	NPoint								origin;
	NSize								size;
};




#endif // NRECTANGLE_HDR


