#!/usr/local/bin/python -w
#==============================================================================
#	NAME:
#		Nano.py
#
#	DESCRIPTION:
#		lldb summarisers for Nano types.
#
#		Import into your ~/.lldbinit to enable:
#
#			command script import /path/to/Nano.py
#
#	COPYRIGHT:
#		Copyright (c) 2006-2021, refNum Software
#		All rights reserved.
#
#		Redistribution and use in source and binary forms, with or without
#		modification, are permitted provided that the following conditions
#		are met:
#		
#		1. Redistributions of source code must retain the above copyright
#		notice, this list of conditions and the following disclaimer.
#		
#		2. Redistributions in binary form must reproduce the above copyright
#		notice, this list of conditions and the following disclaimer in the
#		documentation and/or other materials provided with the distribution.
#		
#		3. Neither the name of the copyright holder nor the names of its
#		contributors may be used to endorse or promote products derived from
#		this software without specific prior written permission.
#		
#		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#==============================================================================
#		Imports
#------------------------------------------------------------------------------
import array
import datetime
import lldb
import math
import os
import re
import struct
import sys





#==============================================================================
#		Constants
#------------------------------------------------------------------------------
# Nano
kNNotFound													= 18446744073709551615

kNTimeNanosecond											= 1.0 / 1000000000.0
kNanoEpochTo1970											= 978307200


kNDataFlagIsLarge											= 0b10000000
kNDataFlagSmallSizeMask										= 0b00011111


kNStringFlagIsLarge											= 0b10000000
kNStringFlagIsSmallUTF16									= 0b01000000
kNStringFlagSmallSizeMask									= 0b00011111


NFileAccess_Read											= 0
NFileAccess_Write											= 1
NFileAccess_ReadWrite										= 2

NFileAccessNames = {
	NFileAccess_Read      : "Read",
	NFileAccess_Write     : "Write",
	NFileAccess_ReadWrite : "ReadWrite",
}


NStringEncoding_Unknown										= 0
NStringEncoding_UTF8										= 1
NStringEncoding_UTF16										= 2
NStringEncoding_UTF32										= 3
NStringEncoding_UTF16BE										= 4
NStringEncoding_UTF16LE										= 5
NStringEncoding_UTF32BE										= 6
NStringEncoding_UTF32LE										= 7
NStringEncoding_ASCII										= 8
NStringEncoding_MacRoman									= 9
NStringEncoding_ISOLatin1									= 10
NStringEncoding_WindowsLatin1								= 11

NStringEncodings = {
	NStringEncoding_Unknown			: { "size" : 1, "name" : "undefined"	},
	NStringEncoding_UTF8			: { "size" : 1, "name" : "utf_8"		},
	NStringEncoding_UTF16			: { "size" : 2, "name" : "utf_16"		},
	NStringEncoding_UTF32			: { "size" : 4, "name" : "utf_32"		},
	NStringEncoding_UTF16BE			: { "size" : 2, "name" : "utf_16_be"	},
	NStringEncoding_UTF16LE			: { "size" : 2, "name" : "utf_16_le"	},
	NStringEncoding_UTF32BE			: { "size" : 4, "name" : "utf_32_be"	},
	NStringEncoding_UTF32LE			: { "size" : 4, "name" : "utf_32_le"	},
	NStringEncoding_ASCII			: { "size" : 1, "name" : "ascii"		},
	NStringEncoding_MacRoman		: { "size" : 1, "name" : "mac_roman"	},
	NStringEncoding_ISOLatin1		: { "size" : 1, "name" : "latin_1"		},
	NStringEncoding_WindowsLatin1	: { "size" : 1, "name" : "cp1252"		}
}


NAnyTypes = {
	# Stored at 8-byte __ptr field
	"5NData"			: "*((NData			*) this->__s.__ptr)",
	"7NString"			: "*((NString		*) this->__s.__ptr)",
	"11NDictionary"		: "*((NDictionary	*) this->__s.__ptr)",


	# Stored in 8-byte __ptr field
	"b"					: "*((bool			*) &this->__s.__ptr)",
	"h"					: "*((uint8_t		*) &this->__s.__ptr)",
	"t"					: "*((uint16_t		*) &this->__s.__ptr)",
	"j"					: "*((uint32_t		*) &this->__s.__ptr)",
	"y"					: "*((uint64_t		*) &this->__s.__ptr)",
	"a"					: "*((int8_t		*) &this->__s.__ptr)",
	"s"					: "*((int16_t		*) &this->__s.__ptr)",
	"i"					: "*((int32_t		*) &this->__s.__ptr)",
	"x"					: "*((int64_t		*) &this->__s.__ptr)",
	"f"					: "*((float32_t		*) &this->__s.__ptr)",
	"d"					: "*((float64_t		*) &this->__s.__ptr)",


	# Stored in 24-byte __buf field
	"5NTime"			: "*((NTime			*) &this->__s.__buf)",
	"6NArray"			: "*((NArray		*) &this->__s.__buf)",
	"7NNumber"			: "*((NNumber		*) &this->__s.__buf)"
}


NVariantTypes = {
	"char"					: "uint8_t",
	"unsigned short"		: "uint16_t",
	"unsigned int"			: "uint32_t",
	"unsigned long"			: "uint32_t",
	"unsigned long long"	: "uint64_t",
	"signed"				: "int8_t",
	"short"					: "int16_t",
	"int"					: "int32_t",
	"long"					: "int32_t",
	"long long"				: "int64_t",
	"float"					: "float32_t",
	"double"				: "float64_t"
}



# Inscrutable objects
#
# lldb will invoke summariser views on objects that are in scope but are
# yet to be initialised.
#
# Parsing these may produce an exception so we trap these exceptions and
# report them as inscrutable.
kInscrutable												= u"\u2754"





#==============================================================================
#		getEncodingSize : Get the code unit size of an encoding.
#------------------------------------------------------------------------------
def getEncodingSize(theEncoding):

	return NStringEncodings.get(theEncoding).get("size")





#==============================================================================
#		getEncodingName : Get the Python name of an encoding.
#------------------------------------------------------------------------------
def getEncodingName(theEncoding):

	return NStringEncodings.get(theEncoding).get("name")





#==============================================================================
#		getMemberUInt : Get a member as an unsigned integer.
#------------------------------------------------------------------------------
def getMemberUInt(sbValue, theName):

	return sbValue.GetChildMemberWithName(theName).GetValueAsUnsigned()





#==============================================================================
#		getMemberFloat : Get a member as a float.
#------------------------------------------------------------------------------
def getMemberFloat(sbValue, theName):

	return float(sbValue.GetChildMemberWithName(theName).GetValue())





#==============================================================================
#		getMemberString : Get a member as a string.
#------------------------------------------------------------------------------
def getMemberString(sbValue, theName):

	return str(sbValue.GetChildMemberWithName(theName).GetSummary().strip('"'))





#==============================================================================
#		getExpressionPathSBValue : Get an expression path as an SBValue.
#------------------------------------------------------------------------------
def getExpressionPathSBValue(sbValue, thePath):

	return sbValue.GetValueForExpressionPath(thePath)





#==============================================================================
#		getExpressionPathSBData : Get an expression path as an SBData.
#------------------------------------------------------------------------------
def getExpressionPathSBData(sbValue, thePath):

	return sbValue.GetValueForExpressionPath(thePath).GetData()





#==============================================================================
#		getExpressionPathUInt : Get an expression path as an unsigned integer.
#------------------------------------------------------------------------------
def getExpressionPathUInt(sbValue, thePath):

	return sbValue.GetValueForExpressionPath(thePath).GetValueAsUnsigned()





#==============================================================================
#		getExpressionPathFloat : Get an expression path as a float.
#------------------------------------------------------------------------------
def getExpressionPathFloat(sbValue, thePath):

	return float(sbValue.GetValueForExpressionPath(thePath).GetValue())





#==============================================================================
#		getMemoryBytes : Get bytes from an address.
#------------------------------------------------------------------------------
def getMemoryBytes(sbValue, theOffset, theSize):

	theProcess  = sbValue.GetProcess()
	theAddresss = sbValue.GetValueAsUnsigned() + theOffset
	theBytes    = theProcess.ReadMemory(theAddresss, theSize, lldb.SBError())

	return theBytes





#==============================================================================
#		getMemoryString : Get a C string from an address.
#------------------------------------------------------------------------------
def getMemoryString(sbValue, maxSize=128):

	theProcess  = sbValue.GetProcess()
	theAddresss = sbValue.GetValueAsUnsigned()
	theString   = theProcess.ReadCStringFromMemory(theAddresss, maxSize, lldb.SBError())
	
	return theString





#==============================================================================
#		getBytesString : Get a string from encoded bytes.
#------------------------------------------------------------------------------
def getBytesString(theBytes, theEncoding):

	pyEncoding = getEncodingName(theEncoding)
	theText    = theBytes.decode(pyEncoding)
	
	return theText





#==============================================================================
#		getSBDataBytes : Get bytes from an SBData.
#------------------------------------------------------------------------------
def getSBDataBytes(sbData, theOffset, theSize):

	theBytes = sbData.ReadRawData(lldb.SBError(), theOffset, theSize)

	return theBytes





#==============================================================================
#		getNDataBytes : Get the bytes from an NData.
#------------------------------------------------------------------------------
def getNDataBytes(theData):

	theFlags = getExpressionPathUInt(theData, "->mData.theFlags")
	isLarge  = (theFlags & kNDataFlagIsLarge)

	if (isLarge):
		theOffset = getExpressionPathUInt(   theData, "->mData.Large.theSlice.mLocation")
		theSize   = getExpressionPathUInt(   theData, "->mData.Large.theSlice.mSize")
		dataPtr   = getExpressionPathSBValue(theData, "->mData.Large.theState->theData")
		theBytes  = getMemoryBytes(dataPtr, theOffset, theSize)

	else:
		theSize  = (theFlags & kNDataFlagSmallSizeMask)
		theBytes = bytes(getExpressionPathSBData(theData, "->mData.Small.theData").uint8s[0:theSize])

	return theBytes





#==============================================================================
#		getFilePath : Get a file path.
#------------------------------------------------------------------------------
def getFilePath(theValue, thePath):

	theString = getExpressionPathSBValue(theValue, thePath)
	thePath   = NString_Summary(theString, None).strip('"')

	if (not thePath):
		thePath = "none"

	return thePath





#==============================================================================
#		formatFloat : Format a float.
#------------------------------------------------------------------------------
def formatFloat(theValue, thePrecision=9):

	theText = "{:." + str(thePrecision) + "g}"
	return theText.format(theValue)





#==============================================================================
#		NAny_Summary : NAny summary.
#------------------------------------------------------------------------------
def NAny_Summary(theObject, theInfo):

	try:
		# Check for validity
		theHandler = getMemberUInt(theObject, "__h")
		if (theHandler == 0):
			return "none"


		# Extract the type
		valueTypeID     = getMemoryString(theObject.EvaluateExpression("this->type().name()"))
		valueExpression = NAnyTypes.get(valueTypeID)

		if (valueExpression == None):
			theValue = "<unknown type '" + valueTypeID + "'>"

		else:
			theValue = str(theObject.EvaluateExpression(valueExpression))
			theValue = re.sub(r"\) \$\d+ = ", ") ", theValue)

		return theValue

	except:
		return kInscrutable





#==============================================================================
#		NArray_Summary : NArray summary.
#------------------------------------------------------------------------------
def NArray_Summary(theObject, theInfo):

	try:
		return "{size = " + str(theObject.GetNumChildren()) + "}"

	except:
		return kInscrutable





#==============================================================================
#		NArray_Contents : NArray contents.
#------------------------------------------------------------------------------
class NArray_Contents:

	def __init__(self, theArray, theInfo):
		theTarget = lldb.debugger.GetSelectedTarget()

		self.theArray = theArray
		self.dataType = theTarget.FindTypes("NAny").GetTypeAtIndex(0)


	def num_children(self):
		return self.numChildren


	def has_children(self):
		return (self.numChildren != 0)


	def get_child_index(self, theName):
		try:
			return int(theName.lstrip("[").rstrip("]"))

		except:
			return -1


	def get_child_at_index(self, theIndex):
		if (theIndex < 0 or theIndex >= self.numChildren):
			return None

		try:
			theOffset = theIndex * self.dataSize
			return self.dataStart.CreateChildAtOffset("[" + str(theIndex) + "]", theOffset, self.dataType)

		except:
			return None


	def update(self):
		try:
			self.numChildren = 0
			self.dataSize    = self.dataType.GetByteSize()
			self.dataStart   = self.theArray.GetChildMemberWithName("__begin_")
			self.dataEnd     = self.theArray.GetChildMemberWithName("__end_")


			ptrStart = self.dataStart.GetValueAsUnsigned()
			ptrEnd   = self.dataEnd.GetValueAsUnsigned()

			if ptrStart != 0 and ptrEnd != 0 and ptrStart < ptrEnd:
				numBytes = ptrEnd - ptrStart

				if ((numBytes % self.dataSize) == 0):
					self.numChildren = int(numBytes / self.dataSize)

		except:
			pass





#==============================================================================
#		NColor_Summary : NColor summary.
#------------------------------------------------------------------------------
def NColor_Summary(theColor, theInfo):

	r = formatFloat(getMemberFloat(theColor, "mRed"),   2)
	g = formatFloat(getMemberFloat(theColor, "mGreen"), 2)
	b = formatFloat(getMemberFloat(theColor, "mBlue"),  2)
	a = formatFloat(getMemberFloat(theColor, "mAlpha"), 2)
	
	return "{r = " + r + ", g = " + g + ", b = " + b + ", a = " + a + "}"





#==============================================================================
#		NData_Summary : NData summary.
#------------------------------------------------------------------------------
def NData_Summary(theData, theInfo):

	try:
		theFlags = getExpressionPathUInt(theData, "->mData.theFlags")
		isLarge  = (theFlags & kNDataFlagIsLarge)

		if (isLarge):
			theSize = getExpressionPathUInt(theData, "->mData.Large.theSlice.mSize")
		
			if (theSize != 0):
				theOffset = getExpressionPathUInt(   theData, "->mData.Large.theSlice.mLocation")
				thePtr    = getExpressionPathSBValue(theData, "->mData.Large.theState->theData")
				theInfo   = ", data = " + hex(thePtr.GetValueAsUnsigned() + theOffset)

		else:
			theSize = (theFlags & kNDataFlagSmallSizeMask)
		
			if (theSize != 0):
				sbData  = getExpressionPathSBData(theData, "->mData.Small.theData")
				theInfo = ", data = {0x" + ", 0x".join(format(x, "02X") for x in sbData.uint8s[0:theSize]) + "}"


		if (theSize == 0):
			return "{size = 0}"

		else:
			return "{size = " + str(theSize) + theInfo + "}"

	except:
		return kInscrutable





#==============================================================================
#		NDate_Summary : NDate summary.
#------------------------------------------------------------------------------
def NDate_Summary(theDate, theInfo):

	theYear  = getMemberUInt(theDate, "mYear")
	theMonth = getMemberUInt(theDate, "mMonth")
	theDay   = getMemberUInt(theDate, "mDay")

	return ("%04d-" % theYear) + ("%02d-" % theMonth) + ("%02d" % theDay)





#==============================================================================
#		NDictionary_Summary : NDictionary summary.
#------------------------------------------------------------------------------
def NDictionary_Summary(theObject, theInfo):

	try:
		return "{size = " + str(theObject.GetNumChildren()) + "}"

	except:
		return kInscrutable





#==============================================================================
#		NDictionary_Contents : NDictionary content.
#------------------------------------------------------------------------------
class NDictionary_Contents:

	def __init__(self, theDictionary, theInfo):
		theTarget = lldb.debugger.GetSelectedTarget()

		self.theDictionary = theDictionary
		self.dataType      = theTarget.FindTypes("NAny").GetTypeAtIndex(0)


	def num_children(self):
		return self.numChildren


	def has_children(self):
		return (self.numChildren != 0)


	def get_child_index(self, theName):
		for n in range(self.numChildren):
			theKey = self.keyValues[n]["key"]
			if (theKey == theName):
				return n
		
		return None


	def get_child_at_index(self, theIndex):
		theInfo  = self.keyValues[theIndex]
		theKey   = theInfo["key"]
		theValue = theInfo["value"]

		return theValue.CreateChildAtOffset("[" + theKey + "]", 0, self.dataType)


	def update(self):
		try:
			self.numChildren = 0

			mapBuckets = self.theDictionary.GetValueForExpressionPath("->__table_.__bucket_list_.__ptr_")
			thePointer = struct.unpack("Q", getMemoryBytes(mapBuckets.AddressOf(), 0, 8))[0]
			isValid    = (thePointer != 0)

			if (isValid):
				self.keyValues = []
				theKeys        = self.theDictionary.EvaluateExpression("this->GetKeys()")
				theValues      = self.theDictionary.EvaluateExpression("this->GetValues()")

				for n in range(theKeys.GetNumChildren()):
					theKey   = theKeys.GetChildAtIndex(n).GetSummary()
					theValue = theValues.GetChildAtIndex(n)

					self.keyValues.append({ "key" : theKey, "value" : theValue })

			self.numChildren = len(self.keyValues)

		except:
			pass





#==============================================================================
#		NFile_Summary : NFile summary.
#------------------------------------------------------------------------------
def NFile_Summary(theFile, theInfo):

	return getFilePath(theFile, "->mInfo.mPath.mPath")





#==============================================================================
#		NFileHandle_Summary : NFileHandle summary.
#------------------------------------------------------------------------------
def NFileHandle_Summary(fileHnd, theInfo):

	thePath = getFilePath(fileHnd, "->mPath.mPath")

	if (thePath != "none"):
		fileAccess = getExpressionPathUInt(fileHnd, "->mAccess")
		thePath    = thePath + " (" + NFileAccessNames.get(fileAccess) + ")"

	return thePath





#==============================================================================
#		NFileInfo_Summary : NFileInfo summary.
#------------------------------------------------------------------------------
def NFileInfo_Summary(fileInfo, theInfo):

	return getFilePath(fileInfo, "->mPath.mPath")





#==============================================================================
#		NFileMap_Summary : NFileMap summary.
#------------------------------------------------------------------------------
def NFileMap_Summary(fileMap, theInfo):

	return getFilePath(fileMap, "->mPath.mPath")





#==============================================================================
#		NFilePath_Summary : NFilePath summary.
#------------------------------------------------------------------------------
def NFilePath_Summary(filePath, theInfo):

	return getFilePath(filePath, "->mPath")





#==============================================================================
#		NNumber_Summary : NNumber summary.
#------------------------------------------------------------------------------
def NNumber_Summary(theNumber, theInfo):

	theInfo = str(theNumber.GetChildMemberWithName("mValue"))
	theInfo = re.sub(r".*mValue = ", "", theInfo)
	
	return theInfo





#==============================================================================
#		NPoint_Summary : NPoint summary.
#------------------------------------------------------------------------------
def NPoint_Summary(thePoint, theInfo):

	x = formatFloat(getMemberFloat(thePoint, "x"))
	y = formatFloat(getMemberFloat(thePoint, "y"))
	
	return "{x = " + x + ", y = " + y + "}"





#==============================================================================
#		NRange_Summary : NRange summary.
#------------------------------------------------------------------------------
def NRange_Summary(theRange, theInfo):

	try:
		theLocation = getMemberUInt(theRange, "mLocation")
		theSize     = getMemberUInt(theRange, "mSize")
	
		theLocation = "kNNotFound" if (theLocation == kNNotFound) else str(theLocation)
		theSize     = "kNNotFound" if (theSize     == kNNotFound) else str(theSize)

		return "{location = " + theLocation + ", size = " + theSize + "}"

	except:
		return kInscrutable





#==============================================================================
#		NRectangle_Summary : NRectangle summary.
#------------------------------------------------------------------------------
def NRectangle_Summary(theRectangle, theInfo):

	x      = formatFloat(getExpressionPathFloat(theRectangle, "->origin.x"))
	y      = formatFloat(getExpressionPathFloat(theRectangle, "->origin.y"))
	width  = formatFloat(getExpressionPathFloat(theRectangle, "->size.width"))
	height = formatFloat(getExpressionPathFloat(theRectangle, "->size.height"))
	
	return "{origin = {x = " + x + ", y = " + y + "}, size = {width = " + width + ", height = " + height + "}}"





#==============================================================================
#		NSize_Summary : NSize summary.
#------------------------------------------------------------------------------
def NSize_Summary(theSize, theInfo):

	width  = formatFloat(getMemberFloat(theSize, "width"))
	height = formatFloat(getMemberFloat(theSize, "height"))
	
	return "{width = " + width + ", height = " + height + "}"





#==============================================================================
#		NString_Summary : NString summary.
#------------------------------------------------------------------------------
def NString_Summary(theString, theInfo):

	try:
		theFlags = getExpressionPathUInt(theString, "->mString.theFlags")
		isLarge  = (theFlags & kNStringFlagIsLarge)
		theInfo  = ""

		if (isLarge):
			# Decode the string
			#
			# Large strings are stored as a null-terminated buffer.
			theEncoding = getExpressionPathUInt(   theString, "->mString.Large.theState->stringData.theEncoding")
			theData     = getExpressionPathSBValue(theString, "->mString.Large.theState->stringData.theData")

			nullSize = getEncodingSize(theEncoding)
			theBytes = getNDataBytes(theData)

			theBytes = theBytes[:-nullSize]
			theInfo  = getBytesString(theBytes, theEncoding)



			# Extract sub-strings
			#
			# A sub-string must be converted to UTF32, sliced by code points, then re-decoded.
			stringSize = getExpressionPathUInt(theString, "->mString.Large.theState->theSize")
			sliceSize  = getExpressionPathUInt(theString, "->mString.Large.theSlice.mSize")

			if (sliceSize != stringSize):
				sliceOffset = getExpressionPathUInt(theString, "->mString.Large.theSlice.mLocation")

				bytesUTF32 = theInfo.encode("utf-32")	
				theFormat  = "=" + "I" * int(len(bytesUTF32) / 4)
				codePoints = struct.unpack(theFormat, bytesUTF32)

				sliceFirst = sliceOffset + 1
				sliceLast  = sliceFirst + sliceSize
				codePoints = codePoints[sliceFirst:sliceLast]

				theFormat  = "=" + "I" * len(codePoints)
				bytesUTF32 = struct.pack(theFormat, *codePoints)
				theInfo    = getBytesString(bytesUTF32, NStringEncoding_UTF32)

		else:
			theSize = (theFlags & kNStringFlagSmallSizeMask)

			if (theSize != 0):
				sbData  = getExpressionPathSBData(theString, "->mString.Small.theData")
				isUTF16 = (theFlags & kNStringFlagIsSmallUTF16)

				if (isUTF16):
					theEncoding = NStringEncoding_UTF16
				else:
					theEncoding = NStringEncoding_UTF8

				byteSize = theSize * getEncodingSize(theEncoding)
				theBytes = getSBDataBytes(sbData, 0, byteSize)
				theInfo  = getBytesString(theBytes, theEncoding)

		return '"' + theInfo + '"'

	except:
		return kInscrutable





#==============================================================================
#		NThreadID_Summary : NThreadID summary.
#------------------------------------------------------------------------------
def NThreadID_Summary(threadID, theInfo):

	theID = getMemberUInt(threadID, "mValue")
	if (theID == 0):
		return "none"

	return hex(theID)





#==============================================================================
#		NTime_Summary : NTime summary.
#------------------------------------------------------------------------------
def NTime_Summary(theTime, theInfo):

	unixSecs = getMemberFloat(theTime, "mValue") + kNanoEpochTo1970
	unixFrac = unixSecs - math.floor(unixSecs)

	strTime = datetime.datetime.utcfromtimestamp(unixSecs).strftime("%Y-%m-%d %H:%M:%S.")
	strSecs = formatFloat(unixFrac)

	if (strSecs != "0"):
		strSecs = strSecs[2:]

	return strTime + strSecs





#==============================================================================
#		NUInt128_Summary : NUInt128 summary.
#------------------------------------------------------------------------------
def NUInt128_Summary(theValue, theInfo):

	valueHi = getMemberUInt(theValue, "mHi")
	valueLo = getMemberUInt(theValue, "mLo")
	theInfo = ("0x%016x" % valueHi) + ("%016x" % valueLo)
	
	return theInfo





#==============================================================================
#		NUTI_Summary : NUTI summary.
#------------------------------------------------------------------------------
def NUTI_Summary(theValue, theInfo):

	theInfo = getMemberString(theValue, "mType")

	return theInfo





#==============================================================================
#		NURL_Summary : NURL summary.
#------------------------------------------------------------------------------
def NURL_Summary(theValue, theInfo):

	theInfo = getMemberString(theValue, "mURL")

	return theInfo





#==============================================================================
#		NVariant_Summary : NVariant summary.
#------------------------------------------------------------------------------
def NVariant_Summary(theVariant, theInfo):

	theType = theVariant.GetType().GetDirectBaseClassAtIndex(0).GetType()
	theInfo = str(theVariant.CreateChildAtOffset("value", 0, theType))

	theMatch = re.search("Active Type = (.*?)\s+{\n  Value = (.*?)\n", theInfo, re.DOTALL)
	if (theMatch == None):
		return "none"

	theType  = theMatch.group(1)
	theValue = theMatch.group(2)

	theType = NVariantTypes.get(theType, theType)
	return "(" + theType + ") " + theValue





#==============================================================================
#		NVector_Summary : NVector summary.
#------------------------------------------------------------------------------
def NVector_Summary(theVector, theInfo):

	x = formatFloat(getMemberFloat(theVector, "x"))
	y = formatFloat(getMemberFloat(theVector, "y"))
	
	return "{x = " + x + ", y = " + y + "}"





#==============================================================================
#		loadNano : Load the Nano summarisers.
#------------------------------------------------------------------------------
def loadNano(theDebugger):

	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NAny_Summary           NAny")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NArray_Summary         NArray")
	theDebugger.HandleCommand("type synthetic add -w Nano -l Nano.NArray_Contents        NArray")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NColor_Summary         NColor")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NData_Summary          NData")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NDate_Summary          NDate")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NDictionary_Summary    NDictionary")
	theDebugger.HandleCommand("type synthetic add -w Nano -l Nano.NDictionary_Contents   NDictionary")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NFile_Summary          NFile")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NFileHandle_Summary    NFileHandle")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NFileInfo_Summary      NFileInfo")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NFileMap_Summary       NFileMap")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NFilePath_Summary      NFilePath")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NNumber_Summary        NNumber")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NPoint_Summary         NPoint")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NRange_Summary         NRange")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NRectangle_Summary     NRectangle")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NSize_Summary          NSize")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NString_Summary        NString")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NThreadID_Summary      NThreadID")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NTime_Summary          NTime")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NUInt128_Summary       NUInt128")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NUTI_Summary           NUTI")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NURL_Summary           NURL")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NVariant_Summary   -x 'NVariant<.*>'")
	theDebugger.HandleCommand("type summary   add -w Nano -F Nano.NVector_Summary        NVector")
	theDebugger.HandleCommand("type category enable Nano")





#==============================================================================
#		__lldb_init_module : Entry point.
#------------------------------------------------------------------------------
def __lldb_init_module(theDebugger, theInfo):

	loadNano(theDebugger)


