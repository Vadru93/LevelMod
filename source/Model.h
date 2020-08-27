#pragma once
#ifndef MODEL_H
#define MODEL_H
#include "defines.h"
#include "Script.h"

struct Model
{
	DWORD** memberFunctions;
	DWORD pNULL;
	DWORD unk1;//0x14
	DWORD unk2;//0x2
	DWORD pNULL2;
	DWORD frameCount;
	Vertex pos;

	DWORD GetChecksum()
	{
		DWORD pChecksum = *(DWORD*)((DWORD)this + 0x98);
		pChecksum = *(DWORD*)(pChecksum + 0xC);
		pChecksum = *(DWORD*)(pChecksum + 4);
		return *(DWORD*)(pChecksum + 0x58);
	}

};

void Obj_MoveToNode(Model* mdl, CStruct* pStruct);
#endif