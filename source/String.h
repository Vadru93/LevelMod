namespace String
{

	extern DWORD numLevelStrings;
	extern char LevelHeapBottom[];
	extern char* LevelHeapTop;

	enum class STRING_HEAP : bool
	{
		PERMANENT = true,
		TEMP = false
	};


	char* AddString(DWORD checksum, const char* str);
	char* AddString(const char* str);
	char* AddLevelString(const char* str);

	void RemoveLevelStrings();

	unsigned long CaseSenseChecksum(const char* str);

	char* CreateString(const char* str, STRING_HEAP heap = STRING_HEAP::TEMP);
};