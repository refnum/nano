/*	NAME:
		NFileHandle.h

	DESCRIPTION:
		File handle.

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
#ifndef NFILE_HANDLE_H
#define NFILE_HANDLE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NString.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NFile;

// Native file handle
using NFileHandleRef = void*;





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Native file handle
static constexpr NFileHandleRef kNFileHandleNone = nullptr;


// File access
//
// A file may be opened for access in one mode at a time:
//
//		NFileAccess::Read			The file may only be read from.
//
//		NFileAccess::Write			The file may only be written to.
//
//		NFileAccess::ReadWrite		The file may be read from and written to.
//
//
// In NFileAccess::Read mode the file will not be created if it does not exist.
//
// In NFileAccess::Write or NFileAccess::ReadWrite modes mode the file will be
// created if it does not exist.
enum class NFileAccess
{
	Read,
	Write,
	ReadWrite
};


// Offsets
enum class NFileOffset
{
	FromStart,
	FromPosition,
	FromEnd
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NFileHandle final
{
public:
										NFileHandle();
									   ~NFileHandle();

										NFileHandle(const NFileHandle&) = delete;
	NFileHandle&                        operator=(  const NFileHandle&) = delete;

										NFileHandle(NFileHandle&&) = delete;
	NFileHandle&                        operator=(  NFileHandle&&) = delete;


	// Is the handle open?
	bool                                IsOpen() const;


	// Get the path
	NString                             GetPath() const;


	// Open/close the handle
	//
	// An open handle must be closed to open it in a different access
	// mode. An open handle will be closed when it goes out of scope.
	NStatus                             Open(const NFile& theFile,   NFileAccess theAccess = NFileAccess::Read);
	NStatus                             Open(const NString& thePath, NFileAccess theAccess = NFileAccess::Read);

	void                                Close();


	// Get/set the position
	//
	// The default position is 0.
	uint64_t                            GetPosition() const;
	NStatus                             SetPosition(int64_t theOffset, NFileOffset relativeTo = NFileOffset::FromStart);


	// Get/set the size
	//
	// A file must be opened for writing to adjust its size.
	//
	// Extending the file size will zero-fill the new content.
	uint64_t                            GetSize() const;
	NStatus                             SetSize(uint64_t theSize);


	// Read from the file
	NStatus                             Read(uint64_t    theSize,
											 void*       thePtr,
											 uint64_t&   numRead,
											 int64_t     theOffset  = 0,
											 NFileOffset relativeTo = NFileOffset::FromPosition);


	// Write to the file
	NStatus                             Write(uint64_t    theSize,
											  const void* thePtr,
											  uint64_t&   numWritten,
											  int64_t     theOffset  = 0,
											  NFileOffset relativeTo = NFileOffset::FromPosition);


	// Flush the file
	//
	// Request that written data be flushed to disk.
	NStatus                             Flush();


private:
	bool                                CanRead()  const;
	bool                                CanWrite() const;

	NStatus                             FileOpen(const NString& thePath, NFileAccess theAccess);
	void                                FileClose();
	uint64_t                            FileGetPosition() const;
	NStatus                             FileSetPosition(int64_t thePosition, NFileOffset relativeTo);
	NStatus                             FileSetSize(uint64_t theSize);
	NStatus                             FileRead(   uint64_t theSize,       void* thePtr, uint64_t& numRead);
	NStatus                             FileWrite(  uint64_t theSize, const void* thePtr, uint64_t& numWritten);
	NStatus                             FileFlush();


private:
	NString                             mPath;
	NFileAccess                         mAccess;
	NFileHandleRef                      mHandle;
};



#endif // NFILE_HANDLE_H
