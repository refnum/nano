/*	NAME:
		LinuxNFileUtils.cpp

	DESCRIPTION:
		Linux file utilities.

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
#include "NFileUtils.h"

// Nano
#include "NPOSIX.h"
#include "NSharedLinux.h"
#include "NSharedPOSIX.h"





//=============================================================================
//		NFileUtils::GetChildren : Get the children of a directory.
//-----------------------------------------------------------------------------
NVectorString NFileUtils::GetChildren(const NString& thePath)
{


	// Get the children
	return NSharedPOSIX::GetChildren(thePath);
}





//=============================================================================
//		NFileUtils::Rename : Atomically rename a file.
//-----------------------------------------------------------------------------
NStatus NFileUtils::Rename(const NString& oldPath, const NString& newPath)
{


	// Rename the file
	return NSharedLinux::FileRename(oldPath, newPath);
}





//=============================================================================
//		NFileUtils::Exchange : Atomically exchange files.
//-----------------------------------------------------------------------------
NStatus NFileUtils::Exchange(const NString& oldPath, const NString& newPath)
{


	// Exchange the files
	return NSharedLinux::FileExchange(oldPath, newPath);
}





#pragma mark private
//=============================================================================
//		NFileUtils::MakeDirectory : Create a directory.
//-----------------------------------------------------------------------------
NStatus NFileUtils::MakeDirectory(const NString& thePath)
{


	// Create the directory
	return NSharedPOSIX::CreateDirectory(thePath);
}





//=============================================================================
//		NFileUtils::DeletePath : Delete a path.
//-----------------------------------------------------------------------------
NStatus NFileUtils::DeletePath(const NString& thePath, bool moveToTrash)
{


	// Validate our parameters
	if (moveToTrash)
	{
		NN_LOG_UNIMPLEMENTED();
		return NStatus::NotSupported;
	}



	// Delete the path
	return NSharedPOSIX::DeletePath(thePath);
}





//=============================================================================
//		NFileUtils::GetLocation : Get a location.
//-----------------------------------------------------------------------------
NStatus NFileUtils::GetLocation(NFileLocation theLocation, NString& thePath)
{


	// Get the location
	NStatus theErr = NStatus::OK;

	switch (theLocation)
	{
		case NFileLocation::AppCaches:
			thePath = NPOSIX::getenv("XDG_CACHE_HOME");
			if (thePath.IsEmpty())
			{
				thePath = NPOSIX::getenv("HOME") + "/.cache";
			}
			break;

		case NFileLocation::AppSupport:
			thePath = NPOSIX::getenv("XDG_DATA_HOME");
			if (thePath.IsEmpty())
			{
				thePath = NPOSIX::getenv("HOME") + "/.local/share";
			}
			break;

		case NFileLocation::AppTemporaries:
			thePath = NPOSIX::getenv("XDG_RUNTIME_DIR");
			if (thePath.IsEmpty())
			{
				thePath = NPOSIX::getenv("TMPDIR");
			}

			if (thePath.IsEmpty())
			{
				thePath = "/tmp";
			}
			break;

		case NFileLocation::SharedSupport:
			thePath = NPOSIX::getenv("XDG_DATA_DIRS");
			if (thePath.Contains(":"))
			{
				thePath = thePath.Split(":").first();
			}

			if (thePath.IsEmpty())
			{
				thePath = "/usr/local/share";
			}
			break;

		case NFileLocation::UserDesktop:
			thePath = NPOSIX::getenv("XDG_DESKTOP_DIR");
			if (thePath.IsEmpty())
			{
				thePath = NPOSIX::getenv("HOME") + "/Desktop";
			}
			break;

		case NFileLocation::UserDocuments:
			thePath = NPOSIX::getenv("XDG_DOCUMENTS_DIR");
			if (thePath.IsEmpty())
			{
				thePath = NPOSIX::getenv("HOME") + "/Documents";
			}
			break;

		case NFileLocation::UserDownloads:
			thePath = NPOSIX::getenv("XDG_DOWNLOAD_DIR");
			if (thePath.IsEmpty())
			{
				thePath = NPOSIX::getenv("HOME") + "/Downloads";
			}
			break;

		case NFileLocation::UserHome:
			thePath = NPOSIX::getenv("HOME");
			break;

		case NFileLocation::UserLogs:
			thePath = NPOSIX::getenv("XDG_DATA_HOME");
			if (thePath.IsEmpty())
			{
				thePath = NPOSIX::getenv("HOME") + "/.local/share/logs";
			}
			break;

		case NFileLocation::UserPictures:
			thePath = NPOSIX::getenv("XDG_PICTURES_DIR");
			if (thePath.IsEmpty())
			{
				thePath = NPOSIX::getenv("HOME") + "/Pictures";
			}
			break;

		case NFileLocation::UserPreferences:
			thePath = NPOSIX::getenv("XDG_CONFIG_HOME");
			if (thePath.IsEmpty())
			{
				thePath = NPOSIX::getenv("HOME") + "/.config";
			}
			break;
	}

	return theErr;
}
