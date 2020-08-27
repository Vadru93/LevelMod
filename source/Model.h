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
	BYTE unk3[0x18];
	DWORD nodeIndex;
	BYTE unk4[0x314];
	D3DXMATRIX rotation;

	DWORD GetNodeIndex()
	{
		return nodeIndex;
	}

};

void Obj_MoveToNode(Model* mdl, CStruct* pStruct);
void Obj_FollowPathLinked(Model* mdl, CStruct* pStruct);
#endif