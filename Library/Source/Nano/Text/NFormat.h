/*	NAME:
		NFormat.h

	DESCRIPTION:
		String formatting.

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
#ifndef NFORMAT_H
#define NFORMAT_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NString.h"





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// Format a string
//
// Accepts either std::format-style or printf-style formatting automatically.
//
//		https://fmt.dev/latest/syntax.html
//
//		https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification
//		https://en.cppreference.com/w/c/io/fprintf
//
// Example:
//
//		NString theResult = NFormat("{} {}", "Hello", "World");
//
//		NString theResult = NFormat("%s %s", "Hello", "World");
//
// std::format formatting errors are reported as a constexpr compile error:
//
//		Constexpr variable 'invalid_format' must
//		be initialized by a constant expression.
//
// printf formatting errors are reported as a compile error.
//
#define NFormat(_format, ...)                                               \
	[&]()                                                                   \
	{                                                                       \
		auto _format_message = FMT_STRING(_format);                         \
																			\
		if constexpr (_nn_has_format_specifiers(_format))                   \
		{                                                                   \
			return NFormatPackToString(_format_message, ##__VA_ARGS__);     \
		}                                                                   \
		else                                                                \
		{                                                                   \
			NN_FORMAT_CHECK_PRINTF(_format, __VA_ARGS__);                   \
			return NSprintfPackToString(_format_message, ##__VA_ARGS__);    \
		}                                                                   \
	}()





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NSimpleFormatter
{
public:
	// Parse the context
	//
	// Base class for formatters that have no formatting specifiers,
	// and who produce their output with a simple "{}".
	constexpr auto                      parse(fmt::format_parse_context& theContext) const;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFormat.inl"



#endif // NFORMAT_H
