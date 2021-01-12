#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "defines.h"
#include "CustomShaders.h"


bool Slerp::transfer = false;
bool Slerp::bDisallowTransfer = false;
bool Slerp::landing = false;
bool Slerp::m_began_frame_in_transfer = false;
bool Slerp::slerping = false;
Matrix Slerp::start;
Matrix Slerp::end;
Matrix Slerp::old;
float Slerp::duration = 0.0f;
float Slerp::radians = 0.0f;
float Slerp::timer = 0.0f;
D3DXVECTOR3 Slerp::axis = D3DXVECTOR3(0, 0, 0);
Vertex Slerp::vel = Vertex(0, 0, 0);
Vertex Slerp::facing = Vertex(0, 0, 0);
float Slerp::height = 0.0f;
Vertex Slerp::goal = Vertex(0, 0, 0);
//
Vertex Slerp::last = Vertex(0, 0, 0);
float Slerp::lerp = 0.0f;
bool Slerp::inAcid = false;
bool Slerp::vert = false;
bool Slerp::addedvel = false;
bool Slerp::landed = false;
bool Slerp::OnGround = false;
bool Slerp::OnGrind = false;
bool Slerp::done = false;
bool Slerp::trying = false;
float Slerp::value = 0.0f;
bool Slerp::wallplant = false;
LARGE_INTEGER Slerp::m_last_wallplant_time_stamp;
Vertex Slerp::realVelocity = Vertex(0, 0, 0);
TransferType Slerp::type = ACID;
Vertex Slerp::targetNormal = Vertex(0, 0, 0);
D3DXVECTOR4 Slerp::target_normal = D3DXVECTOR4(0, 0, 0, 0);

float Slerp::speed = 0.0f;

EXTERN bool GameState::GotSuperSectors = false;
Skater* __restrict Game::skater = NULL;
bool(*Game::PlaySound)(CStruct*, CScript*) = NULL;
DWORD Game::level_checksum = 0;

ShaderObject* Gfx::shaders = NULL;
DWORD Gfx::numMaterials = 0;
D3DMATERIAL9* Gfx::materials = NULL;
Animation* Gfx::animations = NULL;
DWORD Gfx::numAnimations = 0;
DWORD Gfx::numShaders = 0;
bool Gfx::loadingShaders = false;
bool Gfx::loadedShaders = false;
DWORD Gfx::AntiAliasing = 1;
bool Gfx::filtering = true;
DWORD Gfx::numBackBuffers = 2;
bool Gfx::fps_fix = true;
D3DMATERIAL9* Gfx::oldMaterial = NULL;
float Gfx::uv_anim_threshold = 0.01f;
float Gfx::uv_tiling_threshold = 1.0f;
float Gfx::shatter_speed = 0.48f;
float Gfx::shatter_life_factor = 0.5f;
float Gfx::shatter_gravity = 128.0f;
DWORD Gfx::frameCounter = 0;
D3DVIEWPORT9 Gfx::world_viewport;
LPDIRECT3DSURFACE9 Gfx::world_rendertarget = NULL;
D3DXVECTOR3 Gfx::sun_position;
bool Gfx::bOnReset = false;
float Gfx::uv_anim_timer = 0;
HWND Gfx::hFocusWindow = 0;
DWORD Gfx::width = 0;
DWORD Gfx::height = 0;
Gfx::Command Gfx::command = Gfx::Command::None;

bool GameState::IsActive()
{
    return *(bool*)0x00850F74;
}

EXTERN DWORD _GetCurrentTime()
{
    static const DWORD timer = 0x00409AE0;
    _asm call timer
    static DWORD temp = 0;
    _asm mov temp, eax
    return temp;
}