#include "pch.h"
#include "Model.h"
#include "Node.h"
#include "SuperSectors.h"


__declspec(naked) void Obj_MoveToNode_Naked()
{
	static Model* pModel;
	static CStruct* pStruct;
	static DWORD pOldESP;
	static DWORD pOldEBP;
	static DWORD pCall = 0x0048B6E0;
	static DWORD pJmp = 0x0048E03A;
	_asm mov pModel, ecx;
	_asm mov pStruct, eax;
	_asm mov pOldESP, esp;
	_asm mov pOldEBP, ebp;
	Obj_MoveToNode(pModel, pStruct);
	_asm mov ecx, pModel;
	_asm mov esp, pOldESP;
	_asm mov ebp, pOldEBP;
	_asm call[pCall];
	_asm jmp pJmp;
}

void Obj_MoveToNode(Model* mdl, CStruct* pStruct)
{
	DWORD checksum = mdl->GetChecksum();

	CStructHeader* node = Node::GetNodeStruct(checksum);

	if (node)
	{
		CStructHeader* collision;
		if (node->GetStruct(Checksums::Collision, &collision))
		{
			SuperSector* sector = SuperSector::GetSuperSector(collision->Data);
			if (sector)
			{
				_printf(__FUNCTION__ " -> Going to move collision...\n");

				CStructHeader* pNode;
				if (pStruct->GetStruct(Checksums::Name, &pNode))
					sector->MoveToNode(pNode->Data);
				else
					_printf("Need param <Name> in " __FUNCTION__ "\n");

			}
			else
			{
				_printf("Couldn't find SuperSector %s in " __FUNCTION__ "\n", FindChecksumName(collision->Data));
			}
		}
	}
	else
		_printf("Couldn't find Node %s in " __FUNCTION__ "\n", FindChecksumName(checksum));
}

void Obj_FollowPathLinked(Model* mdl)
{
	DWORD checksum = mdl->GetChecksum();

	CStructHeader* node = Node::GetNodeStruct(checksum);

	if (node)
	{
		CStructHeader* collision;
		if (node->GetStruct(Checksums::Collision, &collision))
		{
			SuperSector* sector = SuperSector::GetSuperSector(collision->Data);
			if (sector)
			{
				_printf(__FUNCTION__ " -> Going to move collision...\n");

				movingObjects.push_back(MovingObject(sector, mdl));
			}
			else
			{
				_printf("Couldn't find SuperSector %s in " __FUNCTION__ "\n", FindChecksumName(collision->Data));
			}
		}
	}
	else
		_printf("Couldn't find Node %s in " __FUNCTION__ "\n", FindChecksumName(checksum));
}