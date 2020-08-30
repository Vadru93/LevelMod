#pragma once
#ifndef _STRING_LM_H
#define _STRING_LM_H
namespace String
{

	extern DWORD numLevelStrings;
	extern char LevelHeapBottom[];
	extern char* LevelHeapTop;

	struct PermanentString
	{
		DWORD checksum;
		char* pStr;
	};


	enum class STRING_HEAP : bool
	{
		PERMANENT = true,
		TEMP = false
	};

	enum class HEAP
	{
		ORIGINAL,
		NEW_NOEXTRA,
		NEW_EXTRA,
		LEVEL
	};

	DWORD GetNumStringsTotal();
	DWORD GetNumMaxStringsTotal();
	DWORD GetNumStrings(HEAP heap);
	DWORD GetHeapSize(HEAP heap);
	DWORD GeHeapMaxSize(HEAP heap);


	char* AddString(DWORD checksum, const char* str);
	char* AddString(const char* str);
	char* AddLevelString(const char* str);

	void RemoveLevelStrings();

	unsigned long CaseSenseChecksum(const char* str);

	char* CreateString(const char* str, STRING_HEAP heap = STRING_HEAP::TEMP);
};
#endif