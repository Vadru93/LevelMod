#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Extension\defines.h"
#include "Render\CustomShaders.h"


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
DWORD Slerp::m_last_wallplant_time_stamp;
Vertex Slerp::realVelocity = Vertex(0, 0, 0);
TransferType Slerp::type = ACID;
Vertex Slerp::targetNormal = Vertex(0, 0, 0);
D3DXVECTOR4 Slerp::target_normal = D3DXVECTOR4(0, 0, 0, 0);

float Slerp::speed = 0.0f;

bool GameState::GotSuperSectors = false;
bool GameState::loading_completed = false;
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
BYTE Gfx::fps_fix = true;
bool Gfx::bVSync = true;
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
double Gfx::exact_high_diff = 0.0008333333333333333333333333333;
double Gfx::exact_high = (1000000.0 / Gfx::target_fps) + Gfx::exact_high_diff;
double Gfx::exact_low_diff = 2.966666666666666666666666666667;
double Gfx::exact_low = (1000000.0 / Gfx::target_fps) - Gfx::exact_low_diff;
double Gfx::hybrid_high_diff = 0.133333333333333333333333333333;
double Gfx::hybrid_high = (1000000.0 / Gfx::target_fps) + Gfx::hybrid_high_diff;
double Gfx::hybrid_low_diff = 0.133333333333333333333333333333;
double Gfx::hybrid_low = (1000000.0 / Gfx::target_fps) - Gfx::hybrid_low_diff;
double Gfx::sleep_high_diff = 0.0276666666666666666666666666667;
double Gfx::sleep_high = (1000000.0 / Gfx::target_fps) - Gfx::sleep_high_diff;
double Gfx::sleep_low_diff = 4.186666666666666666666666666667;
double Gfx::sleep_low = (1000000.0 / Gfx::target_fps) - Gfx::sleep_low_diff;
double Gfx::target_fps = 60.0;
double Gfx::frame_modifier = 0.0;
DWORD Gfx::num_threads = 1;
bool Gfx::bOldWindowed = false;
RECT Gfx::window_rect = RECT(50,50,0,0);

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