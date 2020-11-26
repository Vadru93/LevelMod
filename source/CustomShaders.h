#pragma once
#ifndef CUSTOM_SHADERS_H
#define CUSTOM_SHADERS_H
#ifndef NO_DEFINES
#include "d3d9.h"


struct ShaderTexture
{
    LPDIRECT3DTEXTURE9 texture;
    DWORD fixedAlpha;
    DWORD blendMode;
    char texName[128];
};

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


struct ShaderObject
{
    DWORD shaderId;
    DWORD flags;
    union
    {
        Animation* anim;
        float env_tiling[2];
    };
    D3DMATERIAL9* material;
    DWORD blend_op;
    DWORD src_blend;
    DWORD dest_blend;
    DWORD numTextures;
    union
    {
        ShaderTexture* textures;
        DWORD alphaRef;
    };
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
#endif

//--------Custom Shaders--------
namespace Gfx
{
#ifndef NO_DEFINES
    extern bool loadingShaders;
    extern bool loadedShaders;
    extern DWORD numMaterials;
    extern ShaderObject* shaders;
    extern DWORD numShaders;
    extern D3DMATERIAL9* materials;
    extern D3DMATERIAL9* oldMaterial;
    extern Animation* animations;
    extern DWORD numAnimations;
    extern float uv_anim_threshold;
    extern float uv_tiling_threshold;
    extern float shatter_speed;
    void LoadCustomShaders(char* file);
    void UnloadShaders();
#endif
    extern DWORD numBackBuffers;
    extern DWORD AntiAliasing;
    extern bool filtering;
    extern bool fps_fix;
    extern __restrict LPDIRECT3DDEVICE9 pDevice;
    extern bool bOnReset;
};
//--------Custom Shaders--------

#endif