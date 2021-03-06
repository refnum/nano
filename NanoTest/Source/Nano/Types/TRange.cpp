/*	NAME:
		TRange.cpp

	DESCRIPTION:
		NRange tests.

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
// Nano
#include "NFormat.h"
#include "NRange.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const NRange kTestRange1{0, 5};
static const NRange kTestRange2{3, 7};
static const NRange kTestRange3{3, 4};





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TRange)
{
	NRange theRange;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "Default")
{


	// Perform the test
	REQUIRE(theRange.GetLocation() == 0);
	REQUIRE(theRange.GetSize() == 0);

	REQUIRE(kNRangeNone.GetLocation() == kNNotFound);
	REQUIRE(kNRangeNone.GetSize() == 0);

	REQUIRE(kNRangeAll.GetLocation() == 0);
	REQUIRE(kNRangeAll.GetSize() == kNNotFound);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "Intersects")
{


	// Perform the test
	REQUIRE(NRange(4, 3).Intersects(NRange(6, 2)));     // 456 ~ 67 = true   (after)
	REQUIRE(!NRange(4, 3).Intersects(NRange(8, 2)));    // 456 ~ 89 = false  (after gap)
	REQUIRE(NRange(4, 3).Intersects(NRange(3, 2)));     // 456 ~ 34 = true   (before)
	REQUIRE(!NRange(4, 3).Intersects(NRange(1, 2)));    // 456 ~ 12 = false  (before gap)
	REQUIRE(!NRange(4, 3).Intersects(NRange(0, 0)));    // 456 ~ .  = false  (empty)
	REQUIRE(!NRange(4, 3).Intersects(NRange(3, 0)));    // 456 ~ .  = false  (empty)
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "Contains")
{


	// Perform the test
	REQUIRE(!NRange(4, 3).Contains(3));    // 456 ~ 3 = false  (before)
	REQUIRE(NRange(4, 3).Contains(4));     // 456 ~ 4 = true   (first)
	REQUIRE(NRange(4, 3).Contains(5));     // 456 ~ 5 = true   (inside)
	REQUIRE(NRange(4, 3).Contains(6));     // 456 ~ 6 = true   (last)
	REQUIRE(!NRange(4, 3).Contains(7));    // 456 ~ 7 = false  (after)
	REQUIRE(!NRange(4, 0).Contains(3));    //   . ~ 3 = false  (never)
	REQUIRE(!NRange(4, 0).Contains(4));    //   . ~ 4 = false  (never)
	REQUIRE(!NRange(4, 0).Contains(5));    //   . ~ 5 = false  (never)
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "IsMeta")
{


	// Perform the test
	REQUIRE(!kTestRange1.IsMeta());
	REQUIRE(!kTestRange2.IsMeta());

	REQUIRE(kNRangeNone.IsMeta());
	REQUIRE(kNRangeAll.IsMeta());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "IsEmpty")
{


	// Perform the test
	REQUIRE(kNRangeNone.IsEmpty());

	REQUIRE(!NRange(4, 3).IsEmpty());
	REQUIRE(NRange(4, 0).IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "GetLocation")
{


	// Perform the test
	theRange = kTestRange1;
	REQUIRE(theRange.GetLocation() == kTestRange1.GetLocation());

	theRange.SetLocation(theRange.GetLocation() + 1);
	REQUIRE(theRange.GetLocation() != kTestRange1.GetLocation());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "AddOffset")
{


	// Perform the test
	theRange.SetRange(0, 3);
	theRange.AddOffset(0);
	REQUIRE(theRange == NRange(0, 3));

	theRange.SetRange(0, 3);
	theRange.AddOffset(1);
	REQUIRE(theRange == NRange(1, 3));

	theRange.SetRange(0, 3);
	theRange.AddOffset(2);
	REQUIRE(theRange == NRange(2, 3));


	theRange.SetRange(1, 3);
	theRange.AddOffset(0);
	REQUIRE(theRange == NRange(1, 3));

	theRange.SetRange(2, 3);
	theRange.AddOffset(1);
	REQUIRE(theRange == NRange(3, 3));

	theRange.SetRange(3, 3);
	theRange.AddOffset(2);
	REQUIRE(theRange == NRange(5, 3));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "GetSize")
{


	// Perform the test
	theRange = kTestRange1;
	REQUIRE(theRange.GetSize() == kTestRange1.GetSize());

	theRange.SetSize(theRange.GetSize() + 1);
	REQUIRE(theRange.GetSize() != kTestRange1.GetSize());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "Contents")
{


	// Perform the test
	REQUIRE(kTestRange1.GetLocation() == 0);
	REQUIRE(kTestRange1.GetSize() == 5);

	REQUIRE(kTestRange1.GetPosition(0) == kTestRange1.GetFirst());
	REQUIRE(kTestRange1.GetPosition(1) == kTestRange1.GetFirst() + 1);
	REQUIRE(kTestRange1.GetPosition(2) == kTestRange1.GetFirst() + 2);

	REQUIRE(kTestRange1.GetFirst() == 0);
	REQUIRE(kTestRange1.GetLast() == 4);
	REQUIRE(kTestRange1.GetNext() == 5);


	// Perform the test
	REQUIRE(kTestRange2.GetLocation() == 3);
	REQUIRE(kTestRange2.GetSize() == 7);

	REQUIRE(kTestRange2.GetPosition(0) == kTestRange2.GetFirst());
	REQUIRE(kTestRange2.GetPosition(1) == kTestRange2.GetFirst() + 1);
	REQUIRE(kTestRange2.GetPosition(2) == kTestRange2.GetFirst() + 2);

	REQUIRE(kTestRange2.GetFirst() == 3);
	REQUIRE(kTestRange2.GetLast() == 9);
	REQUIRE(kTestRange2.GetNext() == 10);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "GetOffset")
{


	// Perform the test
	REQUIRE(NRange(0, 3).GetOffset(0) == NRange(0, 3));
	REQUIRE(NRange(0, 3).GetOffset(1) == NRange(1, 3));
	REQUIRE(NRange(0, 3).GetOffset(2) == NRange(2, 3));

	REQUIRE(NRange(1, 3).GetOffset(0) == NRange(1, 3));
	REQUIRE(NRange(2, 3).GetOffset(1) == NRange(3, 3));
	REQUIRE(NRange(3, 3).GetOffset(2) == NRange(5, 3));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "GetUnion")
{


	// Perform the test
	REQUIRE(NRange(4, 3).GetUnion(NRange(6, 2)) == NRange(4, 4));    // 456 + 67 = 4567    (after)
	REQUIRE(NRange(4, 3).GetUnion(NRange(8, 2)) ==
			NRange(4, 6));    // 456 + 89 = 456789  (after gap)
	REQUIRE(NRange(4, 3).GetUnion(NRange(3, 2)) == NRange(3, 4));    // 456 + 34 = 3456    (before)
	REQUIRE(NRange(4, 3).GetUnion(NRange(1, 2)) ==
			NRange(1, 6));    // 456 + 12 = 123456  (before gap)
	REQUIRE(NRange(4, 3).GetUnion(NRange(0, 0)) == NRange(4, 3));    // 456 + .  = 456     (empty)
	REQUIRE(NRange(4, 3).GetUnion(NRange(3, 0)) == NRange(4, 3));    // 456 + .  = 456     (empty)
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "GetIntersection")
{


	// Perform the test
	REQUIRE(NRange(4, 3).GetIntersection(NRange(6, 2)) ==
			NRange(6, 1));    // 456 ~ 67     = 6   (end)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(8, 2)) ==
			NRange(0, 0));    // 456 ~ 89     = .   (after)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(3, 2)) ==
			NRange(4, 1));    // 456 ~ 34     = 4   (start)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(1, 2)) ==
			NRange(0, 0));    // 456 ~ 12     = .   (before)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(5, 1)) ==
			NRange(5, 1));    // 456 ~ 5      = 5   (inside)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(3, 5)) ==
			NRange(4, 3));    // 456 ~ 34567  = 456 (within)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(1, 0)) ==
			NRange(0, 0));    // 456 ~ .      = .   (empty)
	REQUIRE(NRange(0, 0).GetIntersection(NRange(1, 2)) ==
			NRange(0, 0));    //   . ~ 12     = .   (empty)
	REQUIRE(NRange(4, 0).GetIntersection(NRange(1, 0)) ==
			NRange(0, 0));    //   . ~ .      = .   (empty)
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "GetNormalized")
{


	// Perform the test
	theRange = kNRangeNone.GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 0 && theRange.GetSize() == 0));    // Meta (none)

	theRange = kNRangeAll.GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 0 && theRange.GetSize() == 10));    // Meta (all)


	theRange = NRange(3, 7).GetNormalized(10);
	REQUIRE(
		(theRange.GetLocation() == 3 && theRange.GetSize() == 7));    // Within (non-zero length)

	theRange = NRange(3, 9).GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 3 && theRange.GetSize() == 7));    // Within (clamped length)

	theRange = NRange(3, 0).GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 3 && theRange.GetSize() == 0));    // Within (zero length)


	theRange = NRange(30, 7).GetNormalized(10);
	REQUIRE(
		(theRange.GetLocation() == 30 && theRange.GetSize() == 0));    // Outside (non-zero length)

	theRange = NRange(30, 0).GetNormalized(0);
	REQUIRE((theRange.GetLocation() == 30 && theRange.GetSize() == 0));    // Outside (zero length)
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "CompareEqual")
{


	// Perform the test
	REQUIRE(kTestRange1 == kTestRange1);
	REQUIRE(kTestRange1 != kTestRange2);

	REQUIRE(kNRangeNone == kNRangeNone);
	REQUIRE(kNRangeNone != kNRangeAll);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "CompareOrder")
{


	// Perform the test
	REQUIRE(kTestRange1 <= kTestRange1);
	REQUIRE(kTestRange1 < kTestRange2);
	REQUIRE(kTestRange1 < kTestRange3);

	REQUIRE(kTestRange2 > kTestRange1);
	REQUIRE(kTestRange2 >= kTestRange2);
	REQUIRE(kTestRange2 > kTestRange3);

	REQUIRE(kTestRange3 > kTestRange1);
	REQUIRE(kTestRange3 < kTestRange2);
	REQUIRE(kTestRange3 >= kTestRange3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRange, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", kTestRange1) == "{location = 0, size = 5}");
	REQUIRE(NFormat("{}", kTestRange2) == "{location = 3, size = 7}");
	REQUIRE(NFormat("{}", kTestRange3) == "{location = 3, size = 4}");
}
