/*	NAME:
		NCFData.h

	DESCRIPTION:
		CFDataRef wrapper.

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
#ifndef NCFDATA_H
#define NCFDATA_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NCFObject.h"
#include "NData.h"

// System
#include <CoreFoundation/CFData.h>





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NCFData : public NCFObject<CFDataRef>
{
public:
										NCFData(const NData& theData);
										NCFData() = default;


	// Get / set the data
	NData                               GetData(NCFSource theSource = NCFSource::Copy) const;
	bool                                SetData(const NData& theData, NCFSource theSource = NCFSource::Copy);
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NCFMutableData : public NCFObject<CFMutableDataRef>
{
public:
										NCFMutableData(const NData& theData);
										NCFMutableData() = default;


	// Get / set the data
	NData                               GetData(NCFSource theSource = NCFSource::Copy) const;
	bool                                SetData(const NData& theData);
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCFData.inl"



#endif // NCFDATA_H
