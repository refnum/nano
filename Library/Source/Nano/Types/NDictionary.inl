/*	NAME:
		NDictionary.inl

	DESCRIPTION:
		Dictionary object.

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





//=============================================================================
//		NDictionary::Clear : Clear the dictionary.
//-----------------------------------------------------------------------------
inline void NDictionary::Clear()
{


	// Clear the dictionary
	clear();
}





//=============================================================================
//		NDictionary::GetSize : Get the size.
//-----------------------------------------------------------------------------
inline size_t NDictionary::GetSize() const
{


	// Get the size
	return size();
}





#pragma mark private
//=============================================================================
//		NDictionary::GetValue : Get a typed value.
//-----------------------------------------------------------------------------
template<typename T>
T NDictionary::GetValue(const NString& theKey, const NString& theType) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());
	NN_REQUIRE(!theType.IsEmpty());

	NN_UNUSED(theType);



	// Get the value
	auto theIter = find(theKey);
	if (theIter != end())
	{
		const auto& theValue = theIter->second;

		if (theValue.Is<T>())
		{
			return theValue.Get<T>();
		}
	}

	return {};
}
