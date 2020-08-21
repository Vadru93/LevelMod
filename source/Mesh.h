#pragma once
#ifndef MESH_H
#define MESH_H
#undef NO_DEFINES
#include "Defines.h"

struct SuperSector;

enum MeshState
{
	create = 0,
	kill,
	visible,
	invisible,
	shatter,
	unknown,
	update
};

struct Mesh
{
	DWORD state;
	WORD flags;
	WORD numUnk;
	WORD numUnk1;
	WORD padding;
	DWORD* pNull;
	WORD* indices;


	void Update()
	{
		//_printf("Updating VertexBuffer\n");
		state |= 0x200;
    }
};

struct Object
{
	Object* prev;
	Object* next;
	DWORD flags;//Maybe??
	SuperSector* sector;
	DWORD* Obj;//SuperSector+0x1C
	DWORD pMemberFunction;
};
#endif