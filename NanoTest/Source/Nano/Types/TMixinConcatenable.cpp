/*	NAME:
		TMixinConcatenable.cpp

	DESCRIPTION:
		NMixinConcatenable tests.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinConcatenable.h"
#include "NStdAlgorithm.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
class TestConcatenable : public NMixinConcatenable<TestConcatenable>
{
public:
	TestConcatenable(uint8_t valueA, uint8_t valueB)
		: mValues{valueA, valueB}
	{
	}

	TestConcatenable()
	{
	}

	const NVectorUInt8* GetValues() const
	{
		return &mValues;
	}


	// Operators
	const TestConcatenable& operator+=(const TestConcatenable& otherObject)
	{
		nstd::push_back(mValues, otherObject.mValues);
		return *this;
	}


private:
	NVectorUInt8                        mValues;
};





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const TestConcatenable kTestObjectA(1, 2);
static const TestConcatenable kTestObjectB(10, 20);





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TMixinConcatenable){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TMixinConcatenable, "Default")
{
	// Perform the test
	TestConcatenable                    theObject;
	const NVectorUInt8* theValues = theObject.GetValues();

										REQUIRE(theValues->empty());


	theObject +=                        kTestObjectA;
	theValues = theObject.GetValues();

										REQUIRE(theValues->size() == 2);
										REQUIRE(theValues->at(0) == 1);
										REQUIRE(theValues->at(      1) == 2);


	theObject = kTestObjectB +          kTestObjectA;
	theValues = theObject.GetValues();

										REQUIRE(theValues->size() == 4);
										REQUIRE(theValues->at(0) == 10);
										REQUIRE(theValues->at(      1) == 20);
										REQUIRE(theValues->at(      2) == 1);
										REQUIRE(theValues->at(3) == 2);
}