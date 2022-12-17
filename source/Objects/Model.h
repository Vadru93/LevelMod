#pragma once
#ifndef MODEL_H
#define MODEL_H
#include "Extension\defines.h"
#include "Script\Script.h"
#include "RenderWare\RenderWare.h"

struct SkinModel
{
    DWORD** memberFunctions;
    BYTE unk[0x27C];
    RpAtomic* first;
    RpAtomic last;
    DWORD padding;
    RpAtomicContainer atomics[0x14];
    DWORD numAtomics;
    DWORD unk1;//Always NULL?
    DWORD checksum;
};

struct Model
{
    DWORD** memberFunctions;
    DWORD pNULL;
    DWORD unk1;//0x14 0x2001 seems to be some flag?
    DWORD unk2;//0x2 for normal model 0x5 for bouncy?
    DWORD pNULL2;
    DWORD frameCount;
    Vertex pos;//The origin
    BYTE unk3[0x18];//current position etc??
    DWORD nodeIndex;
    BYTE unk4[0xF8];
    BYTE flags;
    BYTE unk5[0x21B];
    D3DXMATRIX rotation;

    DWORD GetNodeIndex()
    {
        return nodeIndex;
    }

    void RenderCallback()
    {

    }

    void PostRenderCallback()
    {

    }
};

void Obj_MoveToNode(Model* mdl, CStruct* pStruct);
void Obj_FollowPathLinked(Model* mdl, CStruct* pStruct);
void BouncyObj_Go(Model* mdl);
void BouncyObj_OnBounce(Model* mdl);
#endif