#pragma once
#ifndef QBKEY_H
#define QBKEY_H
//this struct needed to be moved to a seperate file so I could include only this part to d3d8
struct CStruct;
struct CArray;
struct CScript;
struct CStructHeader;
struct D3DXVECTOR3;


#define OTHER_SIZE 0x140000
#define OLD_OTHER_SIZE 0x14000
#define HASH_SIZE 0x63AD0
#define OLD_HASH_SIZE 0xBFF4
#define MAX_TRIGGERS OTHER_SIZE / 0x14
#define OLD_MAX_TRIGGERS OLD_OTHER_SIZE / 0x14
#ifdef EXPORTING
#define EXTERN __declspec(dllexport)
#else
#define EXTERN __declspec(dllimport)
#pragma comment(lib, "LevelMod.lib")
#endif
void EXTERN SetStructValues(CStructHeader*, CStructHeader*);

struct QBKeyHeader//Used to store information about QBKeys (Type, Value/Address)
{
	const enum QBKeyType
	{
		UNDEFINED = 0,
		INT = 1,
		FLOAT = 2,
		STRING = 3,
		LOCAL_STRING = 4,
		PAIR = 5,
		VECTOR = 6,
		SCRIPTED_FUNCTION = 7,
		COMPILED_FUNCTION = 8,
		GLOBAL = 9,
		STRUCT = 10,
		LOCAL_STRUCT = 11,
		ARRAY = 12,
		LOCAL = 13,
	};

	int QBKeyCheck;//First 20 bit of the QBKey
	//Types:
	//0x0 - Undefined - End of array / struct and used when parsing / calling scripts
	//0x1 - Int
	//0x2 - Float
	//0x3 - String
	//0x4 - Local String - Used in struct/array	
	//0x5 - Pair
	//0x6 - Vector
	//0x7 - Scripted Function
	//0x8 - Compiled Function
	//0x9 - Global Variable/Function - Member Function in Thps4
	//0xA - Struct
	//0xB - Local Struct - Used in struct/array Structure Pointer in Thps4
	//0xC - Array
	//0xD - Local Variable/Function - Used in struct/array Name in Thps4
	QBKeyType type;
	int id;//Always 0x2?
	union
	{
		//This is the Value/Address of the QBKey ex:
		//if Type is int this is the value of the int
		//if Type is function this is the address of the function
		//values:
		union
		{
			float f;
			int i;
		} value;
		//pointers:
		char* pStr;
		CScript* pScript;
		D3DXVECTOR3* pVec;
		CArray* pArray;
		bool(*pFunction)(CStruct*, CScript*);
		CStructHeader** pStruct;
	};
	QBKeyHeader* NextHeader;
	inline float GetFloat()
	{
		return value.f;
	}
	inline int GetInt()
	{
		return value.i;
	}
	inline char* GetString()
	{
		return pStr;
	}
	inline CScript* GetScript()
	{
		return pScript;
	}

	EXTERN bool SetFloat(DWORD checksum, float value);
	
	inline bool CallScript(CStruct* pStruct, CScript* pScript)
	{
		return pFunction(pStruct, pScript);
	}
	void SetValues(CStructHeader* values)
	{
		switch (type)
		{
		case STRUCT:
		case LOCAL_STRUCT:
			SetStructValues(*pStruct, values);
		}
	}
};
#endif