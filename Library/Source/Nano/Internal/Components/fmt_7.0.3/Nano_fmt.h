/*	NAME:
		Nano_fmt.h

	DESCRIPTION:
		fmt support.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
#ifndef NANO_FMT_H
#define NANO_FMT_H
//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
#define FMT_HEADER_ONLY





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoMacros.h"

NN_DIAGNOSTIC_PUSH();
NN_DIAGNOSTIC_IGNORE_CLANG("-Wsign-conversion");
NN_DIAGNOSTIC_IGNORE_CLANG("-Wsigned-enum-bitfield");
NN_DIAGNOSTIC_IGNORE_MSVC(4355);    // 'this' used in base member initializer
NN_DIAGNOSTIC_IGNORE_MSVC(4365);    // Signed / unsigned mismatch
NN_DIAGNOSTIC_IGNORE_MSVC(4582);    // Constructor is not implicitly called

#if !NN_DEBUG && !defined(NDEBUG)
	#define NDEBUG
#endif

#include "fmt_format.h"

NN_DIAGNOSTIC_POP();

NN_DIAGNOSTIC_IGNORE_CLANG("-Wdisabled-macro-expansion");
NN_DIAGNOSTIC_IGNORE_CLANG("-Wunused-member-function");


#endif // NANO_FMT_H