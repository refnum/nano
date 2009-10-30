/*	NAME:
		NTargetSystem.h

	DESCRIPTION:
		System support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_SYSTEM_HDR
#define NTARGET_SYSTEM_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NFile.h"
#include "NTask.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetSystem {
public:
	// Find a bundle
	//
	// Returns the application bundle if bundleID is empty.
	static NFile						FindBundle(const NString &bundleID);


	// Tasks
	static TaskInfo						TaskCreate(const NString &theCmd, const NStringList &theArgs);
	static void							TaskDestroy(    TaskInfo &theTask);
	static void							TaskUpdate(     TaskInfo &theTask);
	static NString						TaskRead(       TaskInfo &theTask, bool stdErr=false);
	static void							TaskWrite(const TaskInfo &theTask, const NString &theText);
	static void							TaskWait( const TaskInfo &theTask, NTime theDelay);
	static void							TaskKill( const TaskInfo &theTask);


};





#endif // NTARGET_SYSTEM_HDR
