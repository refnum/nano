/*	NAME:
		TPoint.cpp

	DESCRIPTION:
		NPoint tests.

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
#include "NPoint.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TPoint)
{
	NPoint thePoint;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "Default")
{


	// Perform the test
	REQUIRE(thePoint.x == 0.0);
	REQUIRE(thePoint.y == 0.0);
	REQUIRE(sizeof(thePoint) == 16);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "Constructor")
{


	// Perform the test
	thePoint = NPoint(1, 2);
	REQUIRE(thePoint.x == 1.0);
	REQUIRE(thePoint.y == 2.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "Clear")
{


	// Perform the test
	thePoint = NPoint(1, 2);
	REQUIRE(thePoint.x == 1.0);
	REQUIRE(thePoint.y == 2.0);

	thePoint.Clear();
	REQUIRE(thePoint.x == 0.0);
	REQUIRE(thePoint.y == 0.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "IsZero")
{


	// Perform the test
	REQUIRE(thePoint.IsZero());

	thePoint = NPoint(1, 2);
	REQUIRE(!thePoint.IsZero());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "GetDistance")
{


	// Perform the test
	NPoint pointA(1, 1);
	NPoint pointB(10, 10);

	REQUIRE(NFormat("{:.5f}", pointA.GetDistance(pointB)) == "12.72792");
	REQUIRE(NFormat("{:.5f}", pointA.GetDistance2(pointB)) == "162.00000");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "GetIntegral")
{


	// Perform the test
	thePoint = NPoint(1, 2);
	REQUIRE(thePoint.GetIntegral() == NPoint(1, 2));

	thePoint = NPoint(1.4, 2.5);
	REQUIRE(thePoint.GetIntegral() == NPoint(1, 3));

	thePoint = NPoint(1.5, 2.6);
	REQUIRE(thePoint.GetIntegral() == NPoint(2, 3));


	thePoint = NPoint(1, 2);
	thePoint.MakeIntegral();
	REQUIRE(thePoint == NPoint(1, 2));

	thePoint = NPoint(1.4, 2.5);
	thePoint.MakeIntegral();
	REQUIRE(thePoint == NPoint(1, 3));

	thePoint = NPoint(1.5, 2.6);
	thePoint.MakeIntegral();
	REQUIRE(thePoint == NPoint(2, 3));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "GetOffset")
{


	// Perform the test
	thePoint = NPoint(1, 2);
	REQUIRE(thePoint.GetOffset(-1, -2) == NPoint(0, 0));

	thePoint = NPoint(1.4, 2.5);
	REQUIRE(thePoint.GetOffset(1, 2) == NPoint(2.4, 4.5));


	thePoint = NPoint(1, 2);
	thePoint.Offset(-1, -2);
	REQUIRE(thePoint == NPoint(0, 0));

	thePoint = NPoint(1.4, 2.5);
	thePoint.Offset(1, 2);
	REQUIRE(thePoint == NPoint(2.4, 4.5));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "Addition")
{


	// Perform the test
	NVector theVector(3.1, -8.1);

	thePoint = NPoint(1, 2) + theVector;
	REQUIRE(thePoint == NPoint(4.1, -6.1));

	thePoint = NPoint(1, 2);
	thePoint += theVector;
	REQUIRE(thePoint == NPoint(4.1, -6.1));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "Subtraction")
{


	// Perform the test
	NVector theVector(3.1, -8.1);

	thePoint = NPoint(1, 2) - theVector;
	REQUIRE(thePoint == NPoint(-2.1, 10.1));

	thePoint = NPoint(1, 2);
	thePoint -= theVector;
	REQUIRE(thePoint == NPoint(-2.1, 10.1));

	NPoint pointA(1, 2);
	NPoint pointB(-2.1, 10.1);
	REQUIRE((pointA - pointB) == theVector);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "CompareEqual")
{


	// Perform the test
	NPoint pointA(3.8, 1.3);
	NPoint pointB(8.1, 4.9);

	REQUIRE(pointA == pointA);
	REQUIRE(pointA != pointB);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "CompareOrder")
{


	// Perform the test
	NPoint pointA(3.8, 1.3);
	NPoint pointB(8.1, 4.9);

	REQUIRE(pointA < pointB);
	REQUIRE(pointB > pointA);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPoint, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", NPoint()) == "{x = 0, y = 0}");
	REQUIRE(NFormat("{}", NPoint(1.5, 2)) == "{x = 1.5, y = 2}");
	REQUIRE(NFormat("{}", NPoint(1, -2.5)) == "{x = 1, y = -2.5}");
}
