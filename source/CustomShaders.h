#pragma once
#ifndef CUSTOM_SHADERS_H
#define CUSTOM_SHADERS_H
#include "d3d9.h"


struct ShaderTexture
{
    LPDIRECT3DTEXTURE9 texture;
    DWORD fixedAlpha;
    DWORD blendMode;
    char texName[128];
};

struct ShaderObject
{
    DWORD shaderId;
    float env_tiling[2];
    D3DMATERIAL9* material;
    DWORD blend_op;
    DWORD src_blend;
    DWORD dest_blend;
    DWORD numTextures;
    union
    {
        ShaderTexture* textures;
        DWORD fixedAlpha;
    };
};


//--------Custom Shaders--------
namespace Gfx
{
    extern bool loadingShaders;
    extern bool loadedShaders;
    extern DWORD numMaterials;
    extern ShaderObject* shaders;
    extern DWORD numShaders;
    extern D3DMATERIAL9* materials;
    extern D3DMATERIAL9* oldMaterial;
    void LoadCustomShaders(char* file);
    void UnloadShaders();
};
//--------Custom Shaders--------

#endif