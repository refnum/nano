/*	NAME:
		TVector.cpp

	DESCRIPTION:
		NVector tests.

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
#include "NFormat.h"
#include "NTestFixture.h"
#include "NVector.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TVector)
{
	NVector theVector;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "Default")
{


	// Perform the test
	REQUIRE(theVector.x == 0.0);
	REQUIRE(theVector.y == 0.0);
	REQUIRE(sizeof(theVector) == 16);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "Constructor")
{


	// Perform the test
	theVector = NVector(1, 2);
	REQUIRE(theVector.x == 1.0);
	REQUIRE(theVector.y == 2.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "Clear")
{


	// Perform the test
	theVector = NVector(1, 2);
	REQUIRE(theVector.x == 1.0);
	REQUIRE(theVector.y == 2.0);

	theVector.Clear();
	REQUIRE(theVector.x == 0.0);
	REQUIRE(theVector.y == 0.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "IsZero")
{


	// Perform the test
	REQUIRE(theVector.IsZero());

	theVector = NVector(1, 2);
	REQUIRE(!theVector.IsZero());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "IsNormalized")
{


	// Perform the test
	theVector = NVector(1, 0);
	REQUIRE(theVector.IsNormalized());

	theVector = NVector(1, 1);
	REQUIRE(!theVector.IsNormalized());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "GetLength")
{


	// Perform the test
	theVector = NVector(1, 0);
	REQUIRE(theVector.GetLength() == 1);

	theVector = NVector(0, 2);
	REQUIRE(theVector.GetLength() == 2);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "GetLength2")
{


	// Perform the test
	theVector = NVector(1, 0);
	REQUIRE(theVector.GetLength2() == 1);

	theVector = NVector(0, 2);
	REQUIRE(theVector.GetLength2() == 4);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "GetDot")
{


	// Perform the test
	NVector vectorA(1, 0);
	NVector vectorB(0, 1);

	REQUIRE(vectorA.GetDot(vectorB) == 0.0);
	REQUIRE(vectorB.GetDot(vectorA) == 0.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "GetCross")
{


	// Perform the test
	NVector vectorA(1, 0);
	NVector vectorB(0, 1);

	REQUIRE(vectorA.GetCross(vectorB) == 1.0);
	REQUIRE(vectorB.GetCross(vectorA) == -1.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "GetAngle")
{


	// Perform the test
	NVector vectorA(1, 0);
	NVector vectorB(0, 1);

	REQUIRE(vectorA.GetAngle(vectorB) == 90.0);
	REQUIRE(vectorB.GetAngle(vectorA) == -90.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "GetNormalized")
{


	// Perform the test
	theVector = NVector(1, 0);
	REQUIRE(theVector.GetNormalized() == NVector(1, 0));

	theVector = NVector(1, 1);
	REQUIRE(NFormat("{:.10f}", theVector.GetNormalized().x) == "0.7071067812");
	REQUIRE(NFormat("{:.10f}", theVector.GetNormalized().y) == "0.7071067812");


	theVector = NVector(1, 0);
	theVector.Normalize();
	REQUIRE(theVector == NVector(1, 0));

	theVector = NVector(1, 1);
	theVector.Normalize();
	REQUIRE(NFormat("{:.10f}", theVector.x) == "0.7071067812");
	REQUIRE(NFormat("{:.10f}", theVector.y) == "0.7071067812");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "GetNegated")
{


	// Perform the test
	theVector = NVector(1, 2);
	REQUIRE(theVector.GetNegated() == NVector(-1, -2));

	theVector = NVector(-1, 2);
	REQUIRE(theVector.GetNegated() == NVector(1, -2));

	theVector = NVector(-1, -2);
	REQUIRE(theVector.GetNegated() == NVector(1, 2));


	theVector = NVector(1, 2);
	theVector.Negate();
	REQUIRE(theVector == NVector(-1, -2));

	theVector = NVector(-1, 2);
	theVector.Negate();
	REQUIRE(theVector == NVector(1, -2));

	theVector = NVector(-1, -2);
	theVector.Negate();
	REQUIRE(theVector == NVector(1, 2));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "GetScaled")
{


	// Perform the test
	theVector = NVector(1, 2);
	REQUIRE(theVector.GetScaled(1) == NVector(1, 2));

	theVector = NVector(-1, 2);
	REQUIRE(theVector.GetScaled(2) == NVector(-2, 4));

	theVector = NVector(-1, -2);
	REQUIRE(theVector.GetScaled(3) == NVector(-3, -6));


	theVector = NVector(1, 2);
	theVector.Scale(1);
	REQUIRE(theVector == NVector(1, 2));

	theVector = NVector(-1, 2);
	theVector.Scale(2);
	REQUIRE(theVector == NVector(-2, 4));

	theVector = NVector(-1, -2);
	theVector.Scale(3);
	REQUIRE(theVector == NVector(-3, -6));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "Addition")
{


	// Perform the test
	NVector vectorA(3.1, -8.1);
	NVector vectorB(1, 2);

	theVector = vectorA + vectorB;
	REQUIRE(theVector == NVector(4.1, -6.1));

	theVector = vectorA;
	theVector += vectorB;
	REQUIRE(theVector == NVector(4.1, -6.1));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "Subtraction")
{


	// Perform the test
	NVector vectorA(1, 2);
	NVector vectorB(3.1, -8.1);

	theVector = vectorA - vectorB;
	REQUIRE(theVector == NVector(-2.1, 10.1));

	theVector = vectorA;
	theVector -= vectorB;
	REQUIRE(theVector == NVector(-2.1, 10.1));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "CompareEqual")
{


	// Perform the test
	NVector vectorA(3.8, 1.3);
	NVector vectorB(8.1, 4.9);

	REQUIRE(vectorA == vectorA);
	REQUIRE(vectorA != vectorB);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "CompareOrder")
{


	// Perform the test
	NVector vectorA(3.8, 1.3);
	NVector vectorB(8.1, 4.9);

	REQUIRE(vectorA < vectorB);
	REQUIRE(vectorB > vectorA);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TVector, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", NVector()) == "{x = 0, y = 0}");
	REQUIRE(NFormat("{}", NVector(1.5, 2)) == "{x = 1.5, y = 2}");
	REQUIRE(NFormat("{}", NVector(1, -2.5)) == "{x = 1, y = -2.5}");
}
