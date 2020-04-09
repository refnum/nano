/*	NAME:
		NFileUtils.cpp

	DESCRIPTION:
		File utilities.

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
#include "NFileHandle.h"
#include "NFileInfo.h"
#include "NFormat.h"
#include "NString.h"

// System
#include <random>





//=============================================================================
//		NFileUtils::CreateFile : Create a file.
//-----------------------------------------------------------------------------
NStatus NFileUtils::CreateFile(const NFilePath& thePath, bool deleteExisting)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsAbsolute());



	// Delete any existing item
	NStatus theErr = NStatus::OK;

	if (deleteExisting)
	{
		theErr = Delete(thePath);
		if (theErr == NStatus::NotFound)
		{
			theErr = NStatus::OK;
		}
	}



	// Create the file
	if (theErr == NStatus::OK && !NFileInfo(thePath).Exists())
	{
		// Create the parent
		NFilePath theParent = thePath.GetParent();
		if (theParent.IsValid())
		{
			theErr = CreateDirectory(theParent, false);
			NN_EXPECT_NOT_ERR(theErr);
		}



		// Create the file
		if (theErr == NStatus::OK)
		{
			NFileHandle fileHandle;

			theErr = fileHandle.Open(thePath, NFileAccess::WriteOnly);
			NN_EXPECT_NOT_ERR(theErr);
		}
	}

	return theErr;
}





//=============================================================================
//		NFileUtils::CreateDirectory : Create a directory.
//-----------------------------------------------------------------------------
NStatus NFileUtils::CreateDirectory(const NFilePath& thePath, bool deleteExisting)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsAbsolute());



	// Delete any existing item
	NStatus theErr = NStatus::OK;

	if (deleteExisting)
	{
		theErr = Delete(thePath);
		if (theErr == NStatus::NotFound)
		{
			theErr = NStatus::OK;
		}
	}



	// Create the directory
	if (theErr == NStatus::OK && !NFileInfo(thePath).Exists())
	{
		theErr = NStatus::NotFound;
		NFilePath parentPath;

		for (const auto& thePart : thePath.GetParts())
		{
			if (!parentPath.IsValid())
			{
				// Process the root
				parentPath = thePart;
			}
			else
			{
				// Process the directories
				parentPath = parentPath.GetChild(thePart);
				NFileInfo parentInfo(parentPath);


				// Create the parent
				//
				// We create missing directories, skip existing directories,
				// and fail if we encounter something that is not a directory.
				if (!parentInfo.Exists())
				{
					theErr = MakeDirectory(parentPath);
					NN_EXPECT_NOT_ERR(theErr);
				}
				else if (!parentInfo.IsDirectory())
				{
					theErr = NStatus::Duplicate;
				}
				else
				{
					theErr = NStatus::OK;
				}


				// Handle failure
				if (theErr != NStatus::OK)
				{
					break;
				}
			}
		}
	}

	return theErr;
}





//=============================================================================
//		NFileUtils::Delete : Delete a path.
//-----------------------------------------------------------------------------
NStatus NFileUtils::Delete(const NFilePath& thePath, bool moveToTrash)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsAbsolute());



	// Get the state we need
	NStatus   theErr = NStatus::OK;
	NFileInfo theInfo(thePath);



	// Delete children
	//
	// If we have a directory then, unless we're moving it to the trash,
	// we need to delete its children recursively before we can delete it.
	if (!moveToTrash && theInfo.IsDirectory())
	{
		theErr = DeleteChildren(thePath, false);
		NN_EXPECT_NOT_ERR(theErr);
	}



	// Delete the path
	if (theErr == NStatus::OK && theInfo.Exists())
	{
		theErr = DeletePath(thePath, moveToTrash);
	}

	return theErr;
}





//=============================================================================
//		NFileUtils::DeleteChildren : Delete the children of a directory.
//-----------------------------------------------------------------------------
NStatus NFileUtils::DeleteChildren(const NFilePath& thePath, bool moveToTrash)
{


	// Validate our state
	NN_REQUIRE(NFileInfo(thePath).IsDirectory());



	// Delete the children
	NStatus theErr = NStatus::OK;

	for (const auto& childPath : GetChildren(thePath))
	{
		theErr = Delete(childPath, moveToTrash);
		if (theErr != NStatus::OK)
		{
			break;
		}
	}

	return theErr;
}





//=============================================================================
//		NFileUtils::GetLocation : Get a location.
//-----------------------------------------------------------------------------
NFilePath NFileUtils::GetLocation(NFileLocation  theLocation,
								  const NString& theChild,
								  bool           canCreate)
{


	// Validate our parameters
	NN_REQUIRE(theChild.IsEmpty() || NFilePath(theChild).IsRelative());



	// Get the path
	NFilePath thePath = PathLocation(theLocation);

	if (thePath.IsValid() && !theChild.IsEmpty())
	{
		thePath += NFilePath(theChild);
	}



	// Create the directory
	if (thePath.IsValid() && canCreate)
	{
		if (!NFileInfo(thePath).Exists())
		{
			NStatus theErr = CreateDirectory(thePath);
			NN_EXPECT_NOT_ERR(theErr);

			if (theErr != NStatus::OK)
			{
				thePath.Clear();
			}
		}
	}

	return thePath;
}





//=============================================================================
//		NFileUtils::GetUniqueChild : Get a uniquely named file.
//-----------------------------------------------------------------------------
NFilePath NFileUtils::GetUniqueChild(const NFilePath& thePath, const NString baseName)
{


	// Get the state we need
	std::random_device randDevice;
	std::mt19937_64    randUInt64(randDevice());

	NString fileName      = "NFileUtils";
	NString fileExtension = ".tmp";



	// Process the base name
	if (!baseName.IsEmpty())
	{
		NFilePath basePath(baseName);

		fileName      = basePath.GetFilename(false);
		fileExtension = basePath.GetExtension();

		if (!fileExtension.IsEmpty())
		{
			fileExtension = "." + fileExtension;
		}
	}



	// Generate a unique child
	while (true)
	{
		NString uniqueName =
			NFormat("{}_{:08x}{:08x}{}", fileName, randUInt64(), randUInt64(), fileExtension);

		NFileInfo theInfo(thePath.GetChild(uniqueName));

		if (!theInfo.Exists())
		{
			return theInfo.GetPath();
		}
	}
}
