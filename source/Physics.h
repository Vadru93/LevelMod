#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H
#include "_Math.h"
#include "Skater.h"

#define Physics_Wallplant_Min_Approach_Angle 20.0f
#define Physics_Disallow_Rewallplant_Duration 1000
#define Physics_Wallplant_Speed_Loss 225.0f
#define Physics_Wallplant_Min_Exit_Speed 200.0f
#define Physics_Wallplant_Vertical_Exit_Speed 500.0f
#define Physics_Wallplant_Disallow_Grind_Duration 200.0f
#define Physics_Ignore_Ceilings_After_Wallplant_Duration 200.0f
#define Physics_Min_Wallplant_Height 24.0f
#define Physics_Wallplant_Distance_From_Wall 27.6f
#define Physics_Wallplant_Duration 160
#define Physics_Disallow_Rewallpush_Duration 800

void Interporlate(Matrix* result, float delta);
float	SmoothStep(float value);
float CalculateDuration(float target_height, float pos_Y, float vel_Y, Skater* skater);
bool WallplantTimeGreaterThan(CStruct* pParams, CScript* pScript);

#endif
