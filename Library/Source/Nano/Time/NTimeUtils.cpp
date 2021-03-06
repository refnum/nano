/*	NAME:
		NTimeUtils.cpp

	DESCRIPTION:
		Time utilities.

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
#include "NTimeUtils.h"

// Nano
#include "NPOSIX.h"
#include "NanoConstants.h"

// System
#include <math.h>





//=============================================================================
//		NTimeUtils::GetAbsolute : Get an absolute time.
//-----------------------------------------------------------------------------
NTime NTimeUtils::GetAbsolute(NInterval theInterval)
{


	// Validate our parameters
	NN_REQUIRE(theInterval == kNTimeForever || theInterval >= 0.0);



	// Get the absolute time
	if (theInterval == kNTimeForever)
	{
		theInterval = kNTimeDistantFuture;
	}

	return NTimeUtils::GetTime() + theInterval;
}





//=============================================================================
//		NTimeUtils::IsAfter : Check the current time.
//-----------------------------------------------------------------------------
bool NTimeUtils::IsAfter(NTime theTime)
{


	// Check the time
	return NTimeUtils::GetTime() >= theTime;
}





//=============================================================================
//		NTimeUtils::ToTimespec : Convert to a timespec.
//-----------------------------------------------------------------------------
struct timespec NTimeUtils::ToTimespec(NInterval theInterval)
{
	// Convert the value
	NInterval timeSecs = floor(theInterval);
	NInterval timeFrac = theInterval - timeSecs;

	return {time_t(timeSecs), long(timeFrac / kNTimeNanosecond)};
}





//=============================================================================
//		NTimeUtils::ToTmUTC : Convert to a tm in UTC.
//-----------------------------------------------------------------------------
struct tm NTimeUtils::ToTmUTC(NTime theTime)
{
	// Convert the value
	time_t timeUnix = time_t(theTime + kNanoEpochTo1970);

	return NPOSIX::gmtime(timeUnix);
}





//=============================================================================
//		NTimeUtils::ToTmLocal : Convert to a tm in local time.
//-----------------------------------------------------------------------------
struct tm NTimeUtils::ToTmLocal(NTime theTime)
{
	// Convert the value
	time_t timeUnix = time_t(theTime + kNanoEpochTo1970);

	return NPOSIX::localtime(timeUnix);
}





//=============================================================================
//		NTimeUtils::ToInterval : Convert to an NInterval.
//-----------------------------------------------------------------------------
NInterval NTimeUtils::ToInterval(const struct timespec& timeSpec)
{


	// Convert the value
	NInterval timeSecs = NInterval(timeSpec.tv_sec);
	NInterval timeFrac = NInterval(timeSpec.tv_nsec) * kNTimeNanosecond;

	return timeSecs + timeFrac;
}





//=============================================================================
//		NTimeUtils::ToTime : Convert a tm in UTC to an NTime.
//-----------------------------------------------------------------------------
NTime NTimeUtils::ToTime(const struct tm& timeUTC)
{


	// Convert the value
	struct tm localCopy(timeUTC);
	time_t    timeUnix = mktime(&localCopy);

	return NTime(NInterval(timeUnix), kNanoEpochFrom1970);
}
