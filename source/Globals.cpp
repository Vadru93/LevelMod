#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "defines.h"


bool Slerp::transfer = false;
bool Slerp::landing = false;
bool Slerp::m_began_frame_in_transfer = false;
bool Slerp::slerping = false;
Matrix Slerp::start;
Matrix Slerp::end;
Matrix Slerp::old;
float Slerp::duration = 0.0f;
float Slerp::radians = 0.0f;
float Slerp::timer = 0.0f;
D3DXVECTOR3 Slerp::axis = D3DXVECTOR3(0,0,0);
Vertex Slerp::vel = Vertex(0,0,0);
Vertex Slerp::facing = Vertex(0, 0, 0);
float Slerp::height = 0.0f;
Vertex Slerp::goal = Vertex(0, 0, 0);
//
Vertex Slerp::last = Vertex(0,0,0);
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
DWORD Slerp::m_last_wallplant_time_stamp = 0;
Vertex Slerp::realVelocity = Vertex(0,0,0);
TransferType Slerp::type = ACID;
Vertex Slerp::targetNormal = Vertex(0,0,0);
D3DXVECTOR4 Slerp::target_normal = D3DXVECTOR4(0,0,0,0);

float Slerp::speed = 0.0f;

EXTERN bool GameState::GotSuperSectors = false;

EXTERN DWORD _GetCurrentTime()
{
	static const DWORD timer = 0x00409AE0;
	_asm call timer
	static DWORD temp = 0;
	_asm mov temp, eax
	return temp;
}