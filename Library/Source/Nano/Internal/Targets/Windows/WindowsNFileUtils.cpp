/*	NAME:
		WindowsNFileUtils.cpp

	DESCRIPTION:
		Windows file utilities.

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
#include "NSharedWindows.h"
#include "NString.h"

// System
#include <ktmw32.h>





//=============================================================================
//		Libraries
//-----------------------------------------------------------------------------
#pragma comment(lib, "KtmW32.lib")





//=============================================================================
//		NFileUtils::GetChildren : Get the children of a directory.
//-----------------------------------------------------------------------------
NVectorFile NFileUtils::GetChildren(const NString& thePath)
{


	// Open the directory
	WIN32_FIND_DATA dirEntry{};
	NFileList       theFiles;

	NString thePattern = thePath + "\\*";
	HANDLE  theDir     = FindFirstFile(LPCWSTR(thePattern.GetUTF16()), &dirEntry);

	if (theDir == INVALID_HANDLE_VALUE)
	{
		return theFiles;
	}



	// Collect the children
	do
	{
		NString fileName(dirEntry.cFileName);
		if (fileName != "." && fileName != "..")
		{
			theFiles.emplace_back(NFile(thePath + kNPathSeparator + fileName));
		}
	} while (FindNextFile(theDir, &dirEntry));



	// Clean up
	FindClose(theDir);

	return theFiles;
}





//=============================================================================
//		NFileUtils::Rename : Atomically rename a file.
//-----------------------------------------------------------------------------
NStatus NFileUtils::Rename(const NString& oldPath, const NString& newPath)
{


	// Rename the file
	BOOL wasOK = MoveFileTransactedW(LPCWSTR(oldPath.GetUTF16()),
									 LPCWSTR(newPath.GetUTF16()),
									 nullptr,
									 nullptr,
									 MOVEFILE_COPY_ALLOWED,
									 nullptr);
	NN_EXPECT(wasOK);

	return NSharedWindows::GetLastError(wasOK);
}





//=============================================================================
//		NFileUtils::Exchange : Atomically exchange files.
//-----------------------------------------------------------------------------
NStatus NFileUtils::Exchange(const NString& oldPath, const NString& newPath)
{


	// Exchange the files
	NStatus theErr       = NStatus::OK;
	HANDLE  hTransaction = CreateTransaction(nullptr,
											0,
											TRANSACTION_DO_NOT_PROMOTE,
											0,
											0,
											INFINITE,
											L"NFileUtils::Exchange");

	if (hTransaction == nullptr)
	{
		theErr = NStatus::Internal;
	}
	else
	{
		// Rename the files
		NString tmpPath = newPath + ".exchange";

		theErr = Rename(oldPath, tmpPath);

		if (theErr == NStatus::OK)
		{
			theErr = Rename(newPath, oldPath);
		}

		if (theErr == NStatus::OK)
		{
			theErr = Rename(tmpPath, newPath);
		}

		if (theErr == NStatus::OK)
		{
			BOOL wasOK = CommitTransaction(hTransaction);
			theErr     = NSharedWindows::GetLastError(wasOK);
			NN_EXPECT_NOT_ERR(theErr);
		}



		// Clean up
		BOOL wasOK = CloseHandle(hTransaction);
		NN_EXPECT(wasOK);
	}

	return theErr;
}





//=============================================================================
//		NFileUtils::CreateDirectory : Create a directory.
//-----------------------------------------------------------------------------
NStatus NFileUtils::CreateDirectory(const NString& thePath)
{


	// Create the directory
		BOOL wasOK = CreateDirectoryW(LPCWSTR(thePath.GetUTF8()), nullptr))
			   NN_EXPECT(wasOK);

		return NSharedWindows::GetLastError(wasOK);
}