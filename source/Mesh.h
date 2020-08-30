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

//Rename of Mesh to CSector
//This seems to be CSector in thug1src
//In thps3 it was probably called WorldSector
struct CSector
{
	//Not sure which are available in th3
	//
	enum CSectorState
	{
		mCOLLIDE = 0x0001,
		mCLONE = 0x0002,
		mADD_TO_SUPER_SECTORS = 0x0004,					// Cloned sector needs adding to Super Sectors
		mMARKED_FOR_DELETION = 0x0008,					// Cloned object no longer used but not deleted yet
		mREMOVE_FROM_SUPER_SECTORS = 0x0010,					// Take out of Super Sectors w/o deleting
		mINVALID_SUPER_SECTORS = 0x0020,					// Tells if SuperSectors needs updating
		mIN_SUPER_SECTORS = 0x0040,					// Tells if in SuperSectors
		// These 4 used by replay code
		mACTIVE_AT_REPLAY_START = 0x0080,					// Whether the sector is active at the start of the replay
		mVISIBLE_AT_REPLAY_START = 0x0100,					// Whether the sector is visible at the start of the replay
		mSTORED_ACTIVE = 0x0200,					// Stored active status, so that it can be restored at the end of the replay
		mSTORED_VISIBLE = 0x0400,					// Stored visible status, so that it can be restored at the end of the replay
	};

	DWORD state;
	WORD flags;
	WORD numUnk;
	WORD numUnk1;
	WORD padding;
	DWORD* pNull;
	WORD* indices;


	void Update()
	{
		state |= mINVALID_SUPER_SECTORS;
    }
};

struct Mesh
{
	Mesh* prev;
	Mesh* next;
	DWORD flags;//Maybe??
	SuperSector* sector;
	DWORD* Obj;//SuperSector+0x1C
	DWORD pMemberFunction;
};
#endif