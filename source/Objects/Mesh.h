#pragma once
#ifndef MESH_H
#define MESH_H
#undef NO_DEFINES
#include "Extension\Defines.h"
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

struct TextureData
{
    BYTE unk[0x34];
    Direct3DBaseTexture8* base;
    DWORD unk1;
    DWORD alpha;

#define p_old_state1 *(DWORD*)0x009714D4
#define p_old_state2 *(DWORD*)0x009714D0
#define p_old_state3 *(DWORD*)0x00971540
#define p_old_state4 *(DWORD*)0x00971C64
#define p_old_state5 *(DWORD*)0x00972978
#define p_old_state6(x) *(DWORD*)(0x00971548 + x*4)
#define p_old_state7 *(DWORD*)0x00971544
#define p_old_state8 *(DWORD*)0x00971C68

#define p_old_state9(x) *(DWORD*)(0x00971548 + x*4)

#define p_old_state10 *(DWORD*)0x00971C84

#define p_old_state11(x) *(DWORD*)(0x00971548 + x*4)

#define p_some_state1 *(DWORD*)0x005CEF00
#define p_some_state2 *(DWORD*)0x005CEEFC

#define p_old_state12 *(DWORD*)0x00971C54
#define p_old_state13(x) *(DWORD*)(0x00971548 + x*4)

#define p_current_texture(pass) *(DWORD*)(0x009714F0+pass*4)



    void Submit()
    {
        if (!alpha)
        {
            if (p_old_state1)
            {
                p_old_state1 = alpha;
                if (p_old_state2)
                {
                    DWORD old_state5 = p_old_state5;

                    if (p_old_state3 != 0x2)
                    {
                        p_old_state4 = p_some_state1;
                        if (old_state5 < 0x100)
                        {
                            p_old_state6(old_state5) = 0x13;
                            old_state5++;
                        }
                        p_old_state3 = 0x2;
                    }

                    if (p_old_state7 != 0x1)
                    {
                        p_old_state8 = p_some_state2;
                        if (old_state5 < 0x100)
                        {
                            p_old_state9(p_old_state5) = 0x14;
                            old_state5++;
                        }
                    }
                    if (p_old_state10)
                    {
                        p_old_state11(old_state5) = 0x1B;
                        old_state5++;
                        p_old_state10 = 0;
                    }
                    if (p_old_state12)
                    {
                        p_old_state12 = 0;
                        p_old_state13(old_state5) = 0xF;
                        old_state5++;

                    }
                    //p_old_state5 = old_state5;
                }
            }
        }

        Direct3DBaseTexture8* pTexture = base;

        if (pTexture)
        {
            switch (pTexture->GetType())
            {
            case D3DRTYPE_TEXTURE:
                Gfx::pDevice->SetTexture(0, static_cast<Direct3DTexture8*>(pTexture)->GetProxyInterface());
                return;
            case D3DRTYPE_VOLUMETEXTURE:
                Gfx::pDevice->SetTexture(0, static_cast<Direct3DVolumeTexture8*>(pTexture)->GetProxyInterface());
                return;
            case D3DRTYPE_CUBETEXTURE:
                Gfx::pDevice->SetTexture(0, static_cast<Direct3DCubeTexture8*>(pTexture)->GetProxyInterface());
                return;
            default:
                if (p_current_texture(0) != 0)
                    Gfx::pDevice->SetTexture(0, NULL);
                return;
            }
        }
        else if (p_current_texture(0) != 0)
            Gfx::pDevice->SetTexture(0, NULL);
    }
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

    void Submit()
    {
        if (data)
        {
            data->Submit();
        }
        else if (p_current_texture(0) != 0)
        {
            Gfx::pDevice->SetTexture(0, NULL);
        }
    }

    IDirect3DBaseTexture9* GetBaseTexture()
    {
        //debug_print("tex %p p_unk %X base %X\n", this, p_unk[0], *(DWORD*)(p_unk[0] + 0x34));
        if (data)
        {
            Direct3DBaseTexture8* pTexture = data->base;
            if (pTexture)
            {
                switch (pTexture->GetType())
                {
                case D3DRTYPE_TEXTURE:
                    return static_cast<Direct3DTexture8*>(pTexture)->GetProxyInterface();
                case D3DRTYPE_VOLUMETEXTURE:
                    return static_cast<Direct3DVolumeTexture8*>(pTexture)->GetProxyInterface();
                case D3DRTYPE_CUBETEXTURE:
                    return static_cast<Direct3DCubeTexture8*>(pTexture)->GetProxyInterface();
                }
            }
        }
        return NULL;
    }


    TextureData* data;
    BYTE* p_unk[3];
    char tex_name[256];
    DWORD unk[2];
    DWORD uAddress;
    DWORD vAddress;
    ShaderObject2 shader;
};

struct Material
{
    Texture* texture;
    DWORD color;
    DWORD unk;
    float unk2[3];
    bool unk4;
    bool unk5;
    BYTE flag;//8 = 2sided 0x10 = always visible


    static void Submit_Original(Texture* tex)
    {
        typedef void(__cdecl* const pSubmit)(TextureData* data, Texture* tex);
        pSubmit(0x00554EC0)(tex->data, tex);
    }

    void SubmitTextureOnly()
    {
        //debug_print("BaseTexture %p material %p\n", texture->GetBaseTexture(), this);
        //Submit_Original(texture);
        texture->Submit();


        if (p_current_texstage(D3DTSS_ADDRESSU) != texture->uAddress)
        {
            p_current_texstage(D3DTSS_ADDRESSU) = texture->uAddress;
            p_target_texstage(D3DTSS_ADDRESSU) = texture->uAddress;
            Gfx::pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, texture->uAddress);
        }

        if (p_current_texstage(D3DTSS_ADDRESSV) != texture->vAddress)
        {
            p_current_texstage(D3DTSS_ADDRESSV) = texture->vAddress;
            p_target_texstage(D3DTSS_ADDRESSV) = texture->vAddress;
            Gfx::pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, texture->vAddress);
        }
    }

    void Submit()
    {
        //debug_print("BaseTexture %p material %p\n", texture->GetBaseTexture(), this);
        //Submit_Original(texture);
        texture->Submit();

        if (p_current_texstage(D3DTSS_ADDRESSU) != texture->uAddress)
        {
            p_current_texstage(D3DTSS_ADDRESSU) = texture->uAddress;
            p_target_texstage(D3DTSS_ADDRESSU) = texture->uAddress;
            Gfx::pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, texture->uAddress);
        }

        if (p_current_texstage(D3DTSS_ADDRESSV) != texture->vAddress)
        {
            p_current_texstage(D3DTSS_ADDRESSV) = texture->vAddress;
            p_target_texstage(D3DTSS_ADDRESSV) = texture->vAddress;
            Gfx::pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, texture->vAddress);
        }

        if (texture->shader.flags != 0x30303030)
        {
            //debug_print("%X\n", texture->shader.flags);
            if (p_current_renderstate(D3DRS_BLENDOP) != texture->shader.blend_op)
            {
                //Set old blend_op
                p_current_renderstate(D3DRS_BLENDOP) = texture->shader.blend_op;
                p_target_renderstate(D3DRS_BLENDOP) = texture->shader.blend_op;
                Gfx::pDevice->SetRenderState(D3DRS_BLENDOP, texture->shader.blend_op);
            }

            if (p_current_renderstate(D3DRS_SRCBLEND) != texture->shader.src_blend)
            {
                //Set old src_blend
                p_current_renderstate(D3DRS_SRCBLEND) = texture->shader.src_blend;
                p_target_renderstate(D3DRS_SRCBLEND) = texture->shader.src_blend;
                Gfx::pDevice->SetRenderState(D3DRS_SRCBLEND, texture->shader.src_blend);
            }

            if (p_current_renderstate(D3DRS_DESTBLEND) != texture->shader.dest_blend)
            {
                //Set old dest_blend
                p_current_renderstate(D3DRS_DESTBLEND) = texture->shader.dest_blend;
                p_target_renderstate(D3DRS_DESTBLEND) = texture->shader.dest_blend;
                Gfx::pDevice->SetRenderState(D3DRS_DESTBLEND, texture->shader.dest_blend);
            }
        }
        else
        {
            typedef void(__cdecl* pSetTargetRenderStates)(BYTE state, BYTE pass);
            pSetTargetRenderStates(0x0055CE10)(8, 0);

            DWORD target = p_target_renderstate(D3DRS_BLENDOP);
            if (p_current_renderstate(D3DRS_BLENDOP) != target)
            {
                p_current_renderstate(D3DRS_BLENDOP) = target;
                Gfx::pDevice->SetRenderState(D3DRS_BLENDOP, target);
            }

            target = p_target_renderstate(D3DRS_SRCBLEND);
            if (p_current_renderstate(D3DRS_SRCBLEND) != target)
            {
                p_current_renderstate(D3DRS_SRCBLEND) = target;
                Gfx::pDevice->SetRenderState(D3DRS_SRCBLEND, target);
            }

            target = p_target_renderstate(D3DRS_DESTBLEND);
            if (p_current_renderstate(D3DRS_DESTBLEND) != target)
            {
                p_current_renderstate(D3DRS_DESTBLEND) = target;
                Gfx::pDevice->SetRenderState(D3DRS_DESTBLEND, target);
            }

            if (flag)
                target = D3DCULL_NONE;
            else
                target = D3DCULL_CW;

            if (p_current_renderstate(D3DRS_CULLMODE) != target)
            {
                p_current_renderstate(D3DRS_CULLMODE) = target;
                Gfx::pDevice->SetRenderState(D3DRS_CULLMODE, target);
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
    MaterialSplit splits[64];





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
        DWORD numVertices;
        DWORD numIndices;

        Material* material;
        DWORD vertexShader;
        DWORD flags;
        Direct3DIndexBuffer8* indexBuffer;
        Direct3DVertexBuffer8* vertexBuffer;
        DWORD baseIndex;
        DWORD unk2;//1??
        DWORD* punk4;//NULL?

    };
    MaterialSplit splits[64];

    void AddShader(ShaderObject* shader, DWORD matIndex);
};
#endif