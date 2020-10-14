#pragma once
#ifndef MESH_H
#define MESH_H
#undef NO_DEFINES
#include "Defines.h"
#include "d3d8to9\d3d8to9.hpp"

struct SuperSector;


enum MeshState
{
    create = 0,
    kill,
    visible,
    invisible,
    shatter,
    unknown,
    update,
};

    struct Texture
    {
        struct Animation
        {
            float UVel;//0
            float VVel;//4
            float UFrequency;//8//  fsin(UFreq*t+UPhase)*UAmp+t*UVel
            float VFrequency;//12
            float UAmplitude;//16  fsin(amp*360*Uphase*0.02)*UFreq
            float VAmplitude;//20
            float UPhase;//24
            float VPhase;//28

        };

        struct ShaderObject2
        {
            DWORD flags;
            DWORD blend_op;
            DWORD src_blend;
            DWORD dest_blend;
            union
            {
                Animation* anim;
                float env_tiling[2];
            };
            DWORD alphaRef;
        };

        IDirect3DBaseTexture9* GetBaseTexture()
        {
            _printf("tex %p p_unk %X base %X\n", this, p_unk[0], *(DWORD*)(p_unk[0] + 0x34));
            Direct3DBaseTexture8* pTexture = *(Direct3DBaseTexture8**)(p_unk[0] + 0x34);
            switch (pTexture->GetType())
            {
            case D3DRTYPE_TEXTURE:
                return static_cast<Direct3DTexture8*>(pTexture)->GetProxyInterface();
            case D3DRTYPE_VOLUMETEXTURE:
                return static_cast<Direct3DVolumeTexture8*>(pTexture)->GetProxyInterface();
            case D3DRTYPE_CUBETEXTURE:
                return static_cast<Direct3DCubeTexture8*>(pTexture)->GetProxyInterface();
            }

            return NULL;
        }


        BYTE* p_unk[4];
        char tex_name[256];
        DWORD unk[4];
        ShaderObject2 shader;
    };

struct Material
{
    Texture* texture;

    void Submit()
    {
        _printf("BaseTexture %p material %p\n", texture->GetBaseTexture(), this);
        Gfx::pDevice->SetTexture(0, texture->GetBaseTexture());
        
        if (texture->shader.flags != 0x30303030)
        {
            if (p_current_renderstate(D3DRS_BLENDOP) != texture->shader.blend_op)
            {
                //Set old blend_op
                p_current_renderstate(D3DRS_BLENDOP) = texture->shader.blend_op;
                Gfx::pDevice->SetRenderState(D3DRS_BLENDOP, texture->shader.blend_op);
            }

            if (p_current_renderstate(D3DRS_SRCBLEND) != texture->shader.src_blend)
            {
                //Set old src_blend
                p_current_renderstate(D3DRS_SRCBLEND) = texture->shader.src_blend;
                Gfx::pDevice->SetRenderState(D3DRS_SRCBLEND, texture->shader.src_blend);
            }

            if (p_current_renderstate(D3DRS_DESTBLEND) != texture->shader.dest_blend)
            {
                //Set old dest_blend
                p_current_renderstate(D3DRS_DESTBLEND) = texture->shader.dest_blend;
                Gfx::pDevice->SetRenderState(D3DRS_DESTBLEND, texture->shader.dest_blend);
            }
        }
    }
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
    WORD numSplits;
    WORD index;
    DWORD num_total_indices1;
    DWORD* pNull;
    struct MaterialSplit
    {
        //For each split
        WORD* indices;
        DWORD numIndices;
        Material* material;
    };
    MaterialSplit splits[];
    




    void Update()
    {
        state |= mINVALID_SUPER_SECTORS;
    }
};

//extern struct ShaderObject;
struct ShaderObject;


//This is the material split aka RpMesh in RW engine
struct Mesh
{
    //these two pointers changes, maybe because of draworder??
    Mesh* prev;
    Mesh* next;
    DWORD flags;//Maybe??
    SuperSector* sector;
    DWORD* Obj;//SuperSector+0x1C
    DWORD pMemberFunction;
    DWORD unk;//numSplits?

    struct MaterialSplit
    {
        DWORD unk1;//NULL?
        DWORD stride;
        DWORD numIndices;
        DWORD numVertices;
 
        Material* material;
        DWORD vertexShader;
        DWORD flags;
        Direct3DIndexBuffer8* indexBuffer;
        Direct3DVertexBuffer8* vertexBuffer;
        DWORD baseIndex;
        DWORD unk2;//1??
        DWORD* punk4;//NULL?

    };
    MaterialSplit splits[];

    void AddShader(ShaderObject* shader, DWORD matIndex);
};
#endif