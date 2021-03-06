/*	NAME:
		NDataCompressor.h

	DESCRIPTION:
		Data compressor.

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
#ifndef NDATA_COMPRESSOR_H
#define NDATA_COMPRESSOR_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoTypes.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Compression algorithms
//
// Algorithm identifiers may be serialised. They will never change.
enum class NCompression
{
	Null                                                    = 0x6E756C6C,    // 'null'
	ZLib                                                    = 0x7A6C6962     // 'zlib'
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NData;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NDataCompressor
{
public:
	// Compress/decompress data
	static NData                        Compress(  NCompression theCompression, const NData& theData);
	static NData                        Decompress(NCompression theCompression, const NData& theData);


	// Get the maximum size
	//
	// Returns the maximum size required for compressing the specified
	// number of bytes with a particular compression algorithm.
	static size_t                       GetMaxSize(NCompression theCompression, size_t theSize);


private:
	static NData                        Null_Compress(  const NData& theData);
	static NData                        Null_Decompress(const NData& theData);
	static size_t                       Null_MaxSize(size_t theSize);

	static NData                        ZLib_Compress(  const NData& theData);
	static NData                        ZLib_Decompress(const NData& theData);
	static size_t                       ZLib_MaxSize(size_t theSize);
};



#endif // NDATA_COMPRESSOR_H
