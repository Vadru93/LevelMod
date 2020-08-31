#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Physics.h"
#include <stdio.h>
#include <math.h>
#include "Node.h"
#include "Script.h"
#include "Settings.h"

bool WallplantTimeGreaterThan(CStruct* pParams, CScript* pScript)
{
	int time = pParams->GetInt();
	DWORD currentTime = _GetCurrentTime();

	_printf("targetTime %d timeLapsed %d currentTime %d\n", time, currentTime - Slerp::m_last_wallplant_time_stamp, currentTime);
	return (currentTime - Slerp::m_last_wallplant_time_stamp) > time;
}

//Spine and acid stuff, this is based off thug1src
struct SAcidDropData
{
	Vertex target_pos;
	Vertex target_normal;
	float true_target_height;
	bool lerp;
};



__declspec(naked) void BeganTransferCheck_naked()
{
	static DWORD retaddr = 0;
	_asm pop retaddr;
	static const DWORD limit_speed = 0x0049E260;
	_asm call limit_speed
	Slerp::m_began_frame_in_transfer = Slerp::transfer;
	_asm push retaddr;
	_asm ret
}



__declspec(naked) void handle_post_transfer_limit_overrides_naked()
{

}

void ResetTransfer(Skater* skater)
{
	_printf("landing from transfer\n");
	skater->ResetLerpingMatrix();

	if (!Slerp::landing)
	{
		QBKeyHeader* header = GetQBKeyHeader(Checksums::Normal_Lerp_Speed);
		if (header)
			header->value.f = Slerp::value;


		*(Vertex*)skater->GetVelocity() = Slerp::last;
		((Vertex*)skater->GetVelocity())->RotateToNormal(Slerp::goal);
		((Vertex*)skater->GetVelocity())->RotateToPlane(*(Vertex*)skater->GetCollisionNormal());
	}
	/**(Vertex*)skater->GetVelocity() = Slerp::last;
	//skater->GetMatrix() = Slerp::end;
	((Vertex*)skater->GetVelocity())->RotateToNormal(Slerp::goal);
	//skater->RotateToPlane();
	Vertex test_vel = *(Vertex*)skater->GetVelocity();
	//skater->remove_sideways_velocity(&test_vel);
	if (test_vel.y > 0.0f)
	{
	  // use a standard landing instead
	  *((Vertex*)skater->GetVelocity()) = Slerp::last;
	  ((Vertex*)skater->GetVelocity())->ProjectToPlane(*(Vertex*)skater->GetCollisionNormal());
	}*/

	float minSpeed = 500.0f;//bankdropminspeed
	if (((Vertex*)skater->GetVelocity())->LengthSqr() < minSpeed * minSpeed)
	{
		((Vertex*)skater->GetVelocity())->Normalize(minSpeed);
	}
	//*skater->GetVelocity() = -*skater->GetVelocity();
	if (!Slerp::landing)
		skater->SetLanded(Slerp::vert);
	Slerp::transfer = false;
	Slerp::landing = false;
	Slerp::slerping = false;
	//skater->ResetLerpingMatrix();
	Slerp::landed = true;

	/*CStruct params;

	CStructHeader* param = params.AddParam("Speed", QBKeyHeader::QBKeyType::FLOAT);
	param->value.f = Slerp::speed;

	ExecuteQBScript("SetSpeedScripts", &params);*/



	/*QBKeyHeader* header = GetQBKeyHeader(Checksum("SetSpeed"));
	if (header)
		_printf("Set Speed%p  TYPE %s Vel %f %f %f\n", header->pFunction, QBTypes[header->type], skater->GetVelocity()->x, skater->GetVelocity()->y, skater->GetVelocity()->z);
	else
		MessageBox(0, "hmm", "", 0);
	CStruct CSpeed;
	CStructHeader hSpeed;
	hSpeed.Type = QBKeyHeader::QBKeyType::FLOAT;
	hSpeed.QBkey = 0;
	hSpeed.value.f = Slerp::speed;
	CSpeed.head = &hSpeed;
	CSpeed.tail = &hSpeed;
	header->pFunction(&CSpeed, NULL);



	_printf("Set Speed%p  Vel %f %f %f\n", header->pFunction, skater->GetVelocity()->x, skater->GetVelocity()->y, skater->GetVelocity()->z);*/
}

__declspec(naked) void ResetTransfer_naked()
{
	static const DWORD jmpBack = 0x004A3097 + 4;
	static Skater* skater = NULL;
	_asm mov skater, ebp

	_asm pushad;
	//_asm pushfd;
	//_printf("inside resettransfer\n");

	if (Slerp::transfer && skater) [[unlikely]]
	{
		ResetTransfer(skater);
	}
	_asm popad;
	static DWORD unk1 = 0x0040FA20;
	_asm call unk1
	_asm jmp[jmpBack]
}

inline bool is_vert_for_transfers(const Vertex* normal)
{
	//_printf("Vert, %f %f %f\n", normal->x, normal->y, normal->z);
	// cull out non-vert vert polys when looking for spine transfer and acid drop triggers; allows designers to be a little sloppier
	return fabsf(normal->y) < 0.707f;
}

bool look_for_transfer_target(const D3DXVECTOR3& search_dir, const Vertex& start_normal, bool& hip_transfer, Vertex& target, Vertex& target_normal)
{
	// take a bunch of steps forward until we find one		
	// This is not very good, as we have to do 80 collision checks....
	// we really need to optimize our collision detection to be able to select a set of "nearby" object
	// or to select a set that intersects a sphere, or a plane
	// (here, we could just get the set that intersects the plane)
	// this could be statically cached by the colision code, and have one set
	// or perhaps more flexibly, each "feeler" could have a set of objects
	// that it deals with (defaulting to the set of all objects)

	Skater* skater = Skater::GetSkater();


	for (float step = 10.0f; step < 650.0f; step += 5.0f)
	{
		// First find a VERT point a bit in front of us
		// can be some distance below us 
		// allowing us to transfer from high to low pools
		// (and low to high, proving you can jump up from the low point to the high point first)
		/*Vertex vel = *GetVelocity();
		Vertex vel_normal = ::GetNormal(&vel);*/
		Vertex start = *(Vertex*)&(*(Vertex*)skater->GetPosition() + search_dir * step);		// start at current height
		//printf("Start %f %f, pos %f %f\n", start.x, start.z, skater->GetPosition()->x, skater->GetPosition()->z);
		//start.y += 100.0f;m
		Vertex end = start;
		end.y -= 4500.0f;									// long way below
		skater->SetRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end);
		if (skater->CollisionCheck(Collision::Flags::Vert) && is_vert_for_transfers((Vertex*)skater->GetCollisionNormal()))
		{
			_printf("Found Vert\n");
			Vertex horizontal_normal = *(Vertex*)skater->GetCollisionNormal();
			horizontal_normal.y = 0.0f;
			horizontal_normal.Normalize();
			float dot = D3DXVec3Dot((D3DXVECTOR3*)&start_normal, (D3DXVECTOR3*)&horizontal_normal);
			if (dot <= 0.95f)//same as in thug1src
			{
				target = *(Vertex*)skater->GetHitPoint();
				target_normal = *(Vertex*)skater->GetCollisionNormal();
				//_printf("Target %f %f %f normal %f %f %f\n", target.x, target.y, target.z, target_normal.x, target_normal.y, target_normal.z);

				hip_transfer = dot > -0.866f;//same as in thug1src
				if (hip_transfer)
				{
					//Added a check here to see if the two normals are on the same axis and have a low angle
					//Without this check you wil hip transfer to ramps that are on the same horizon as you...
					if (dot >= 0.4f && Sgn(start_normal.x) == Sgn(horizontal_normal.x) && Sgn(start_normal.z) == Sgn(horizontal_normal.z))
					{
						_printf("HIP with too low angle?\n");
						return false;
					}
					_printf("dot %f\nstart %f %f, target %f %f\n", dot, start_normal.x, start_normal.z, horizontal_normal.x, horizontal_normal.z);
				}

				return true;
			}
			else
			{
				target = *(Vertex*)skater->GetHitPoint();
				target_normal = *(Vertex*)skater->GetCollisionNormal();
				_printf("FAlSE Target dot %f\n%f %f %f normal %f %f %f\n", dot, target.x, target.y, target.z, target_normal.x, target_normal.y, target_normal.z);

			}
		}
	}

	return false;
}


void TestInterporlator(Matrix* result, float delta)
{
	/* Cap and floor the delta */
	/* If we are at one end the solution is easy */
	/*if (delta <= 0.0f)
	{
		// delta = 0.0f;
		*result = Slerp::start;
		return;
	}
	else if (delta >= 1.0f)
	{
		// delta = 1.0f;
		*result = Slerp::end;
		return;
	}*/

#if 0
	// GJ:  always lerp, used while slerp was being debugged
	//    m_useLerp = true;
#endif

  /* Do the lerp if we are, else... */
	if (Slerp::lerp && false)
	{
		/* Get the lerp matrix */
		Matrix	lerp;
		D3DXVECTOR4	lpos;
		D3DXVECTOR4	spos;
		D3DXVECTOR4	epos;
		D3DXVECTOR4	rpos;

		D3DXMatrixIdentity(&lerp);

		spos = *(D3DXVECTOR4*)Slerp::start.m[POS];
		epos = *(D3DXVECTOR4*)Slerp::end.m[POS];

		*(D3DXVECTOR3*)lerp.m[RIGHT] = *(D3DXVECTOR3*)Slerp::end.m[RIGHT] - *(D3DXVECTOR3*)Slerp::start.m[RIGHT];
		*(D3DXVECTOR3*)lerp.m[UP] = *(D3DXVECTOR3*)Slerp::end.m[UP] - *(D3DXVECTOR3*)Slerp::start.m[UP];
		*(D3DXVECTOR3*)lerp.m[AT] = *(D3DXVECTOR3*)Slerp::end.m[AT] - *(D3DXVECTOR3*)Slerp::start.m[AT];
		lpos = epos - spos;

		/* Do lerp */
		(*(Vector*)lerp.m[RIGHT]).Scale(delta);
		(*(Vector*)lerp.m[UP]).Scale(delta);
		(*(Vector*)lerp.m[AT]).Scale(delta);
		(*(Vector*)&lpos).Scale(delta);

		*(D3DXVECTOR3*)(*result).m[RIGHT] = *(D3DXVECTOR3*)Slerp::start.m[RIGHT] + *(D3DXVECTOR3*)lerp.m[RIGHT];
		*(D3DXVECTOR3*)(*result).m[UP] = *(D3DXVECTOR3*)Slerp::start.m[UP] + *(D3DXVECTOR3*)lerp.m[UP];
		*(D3DXVECTOR3*)(*result).m[AT] = *(D3DXVECTOR3*)Slerp::start.m[AT] + *(D3DXVECTOR3*)lerp.m[AT];
		rpos = spos + lpos;

		(*(Vertex*)(*result).m[RIGHT]).Normalize();
		(*(Vertex*)(*result).m[UP]).Normalize();
		(*(Vertex*)(*result).m[AT]).Normalize();

		*(D3DXVECTOR4*)(*result).m[POS] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		*(D3DXVECTOR3*)(*result).m[POS] = *(D3DXVECTOR3*)&rpos;
	}
	else
	{
		D3DXVECTOR3	rpos;
		D3DXVECTOR3	spos;
		D3DXVECTOR3	epos;
		D3DXVECTOR3  lpos;

		spos = *(D3DXVECTOR3*)Slerp::start.m[POS];
		epos = *(D3DXVECTOR3*)Slerp::end.m[POS];

		/* Remove the translation for now */
		*result = Slerp::start;
		//*(D3DXVECTOR4*)(*result).m[POS] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

		/* Rotate the new matrix */
		//      m_axis[W] = 0.0f;
		/*Slerp::axis.x = 0.0f;
		Slerp::axis.y = 0.0f;*/
		(*result).Rotate(Slerp::axis, Slerp::radians * delta);//*/D3DXMatrixRotationAxis((D3DXMATRIX*)result, &Slerp::axis, Slerp::radians * delta);//result->Rotate(Slerp::axis, Slerp::radians * delta);

		/* Do linear interpolation on position */
		lpos = epos - spos;
		(*(Vertex*)&lpos).Scale(delta);
		rpos = spos + lpos;

		//*(D3DXVECTOR3*)(*result).m[POS] = rpos;
	}
}


bool Skater::CheckForSpine()
{
	// Break spin button is pressed, so try to break the spine

	// The line to check along is the skater's forward directinal vector, rotated onto the XZ plane
	// if you go straight up the wall, then this will be the same as the normal of the wall (in XY) as we previously calculated
	// however we also want to handle the cases where you approach the QP at an angle

	// Need to take the forward vector (Z) and rotate it "forward" 90 degrees
	// Rotate about an axis perpendicular to both the horizontal part of m_matrix[Y] and also the world up (0,1,0)

	Vertex skater_up = *(Vertex*)GetMatrix()->m[Y];	// skater_up is expected to be horizontal here, as we are "vert"
	skater_up.y = 0.0f;
	skater_up.Normalize();

	// get a vector perpendicular to the plane containing m_matrix[Z] and the world up 	
#if 0 // old code - crossing by axis alined	vector bugs me
	Vertex world_up(0.0f, 1.0f, 0.0f);
	Vertex side = CrossProduct(&skater_up, &world_up);
#else
	Vertex side(-skater_up.z, 0.0f, skater_up.x);
#endif

	// assuming we have not slowed down much, then our velocity should roughly be in the direction we took off from 
	Vertex forward = *(Vertex*)&(-*GetVelocity());
	if (-forward.y < 1.0f)
		return false;
	forward.Normalize();

	Vertex wall_out = forward; 							// forward facing vector
	wall_out.Rotate(side, D3DX_PI / 2.0f);					// rotate fowrad 90 degrees
	wall_out = *(Vertex*)&(-wall_out);

	float speed;
	float dist = 12.0f;
	float time = 1.0f;
	bool hip_transfer = false;

	// Here the "wall" is what we are currently skating on, anything with "wall" in the name refers to that

	Vertex target;
	Vertex target_normal;
	bool target_found = false;

	// First find a point beneath our current position
	// Nice long line, higher than we can posibly jump
	D3DXVECTOR3 start = (*GetPosition() + wall_out * 0.5f);
	D3DXVECTOR3 end = (*GetPosition() + wall_out * 0.5f);
	end.y -= 4500.0f;

	// ignore everything that is NOT vert
	// feeler.SetIgnore(0, mFD_VERT);

	Vertex	wall_pos;
	SetRay(start, end);
	if (CollisionCheck(Collision::Flags::Vert))
	{
		_printf("found target -\n");

		//MessageBox(0, "found wall", "", 0);
		wall_pos = *(Vertex*)GetHitPoint();

		Vertex start_normal = *(Vertex*)&this->normal;
		start_normal.y = 0.0f;
		start_normal.Normalize();

		target_found = look_for_transfer_target(wall_out, start_normal, hip_transfer, target, target_normal);

		if (!target_found)
		{
			Vertex left_along_vert(-start_normal.z, 0.0f, start_normal.x);

			// no target was found in the forward direction, perhaps we should look slightly left or right; look in the horizontal direction which is
			// halfway between the previous search direction and the plane of the vert
			if (!GetKeyState(KeyState::LEFT)->IsPressed() && GetKeyState(KeyState::RIGHT)->IsPressed())
			{
				Vertex search_dir = *(Vertex*)&(-left_along_vert + wall_out);
				search_dir.Normalize();
				target_found = look_for_transfer_target(search_dir, start_normal, hip_transfer, target, target_normal);
			}
			else if (!GetKeyState(KeyState::RIGHT)->IsPressed() && GetKeyState(KeyState::LEFT)->IsPressed())
			{
				Vertex search_dir = *(Vertex*)&(left_along_vert + wall_out);
				search_dir.Normalize();
				target_found = look_for_transfer_target(search_dir, start_normal, hip_transfer, target, target_normal);
			}
		}
	}
	else
	{
		_printf("Retrying\n");
		D3DXVECTOR3 start = (*GetPosition() - wall_out * 0.5f);
		D3DXVECTOR3 end = (*GetPosition() - wall_out * 0.5f);
		end.y -= 4500.0f;

		SetRay(start, end);
		if (CollisionCheck(Collision::Flags::Vert))
		{
			_printf("found target +\n");

			//MessageBox(0, "found wall", "", 0);
			wall_pos = *(Vertex*)GetHitPoint();

			Vertex start_normal = *(Vertex*)&this->normal;
			start_normal.y = 0.0f;
			start_normal.Normalize();

			target_found = look_for_transfer_target(wall_out, start_normal, hip_transfer, target, target_normal);

			if (!target_found)
			{
				Vertex left_along_vert(-start_normal.z, 0.0f, start_normal.x);

				// no target was found in the forward direction, perhaps we should look slightly left or right; look in the horizontal direction which is
				// halfway between the previous search direction and the plane of the vert
				if (!GetKeyState(KeyState::LEFT)->IsPressed() && GetKeyState(KeyState::RIGHT)->IsPressed())
				{
					Vertex search_dir = *(Vertex*)&(-left_along_vert + wall_out);
					search_dir.Normalize();
					target_found = look_for_transfer_target(search_dir, start_normal, hip_transfer, target, target_normal);
				}
				else if (!GetKeyState(KeyState::RIGHT)->IsPressed() && GetKeyState(KeyState::LEFT)->IsPressed())
				{
					Vertex search_dir = *(Vertex*)&(left_along_vert + wall_out);
					search_dir.Normalize();
					target_found = look_for_transfer_target(search_dir, start_normal, hip_transfer, target, target_normal);
				}
			}
		}
		else
		{
			_printf("Retrying\n");
			D3DXVECTOR3 start = (*GetPosition());
			D3DXVECTOR3 end = (*GetPosition());
			end.y -= 4500.0f;

			SetRay(start, end);
			if (CollisionCheck(Collision::Flags::Vert))
			{
				_printf("found target\n");

				//MessageBox(0, "found wall", "", 0);
				wall_pos = *(Vertex*)GetHitPoint();

				Vertex start_normal = *(Vertex*)&this->normal;
				start_normal.y = 0.0f;
				start_normal.Normalize();

				target_found = look_for_transfer_target(wall_out, start_normal, hip_transfer, target, target_normal);

				if (!target_found)
				{
					Vertex left_along_vert(-start_normal.z, 0.0f, start_normal.x);

					// no target was found in the forward direction, perhaps we should look slightly left or right; look in the horizontal direction which is
					// halfway between the previous search direction and the plane of the vert
					if (!GetKeyState(KeyState::LEFT)->IsPressed() && GetKeyState(KeyState::RIGHT)->IsPressed())
					{
						Vertex search_dir = *(Vertex*)&(-left_along_vert + wall_out);
						search_dir.Normalize();
						target_found = look_for_transfer_target(search_dir, start_normal, hip_transfer, target, target_normal);
					}
					else if (!GetKeyState(KeyState::RIGHT)->IsPressed() && GetKeyState(KeyState::LEFT)->IsPressed())
					{
						Vertex search_dir = *(Vertex*)&(left_along_vert + wall_out);
						search_dir.Normalize();
						target_found = look_for_transfer_target(search_dir, start_normal, hip_transfer, target, target_normal);
					}
				}
			}
		}
	}
	/*else
	  AddRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end, D3DXCOLOR(0.0f, 255.0f, 0.0f, 255.0f));*/

	if (!target_found) return false;

	Vertex XZ_to_target = *(Vertex*)&(target - wall_pos);
	XZ_to_target.y = 0.0f;
	dist = XZ_to_target.Length();

	// We are only going to allow this later if the target point is the same level
	// as the takeoff point, and we have a clear line
	// so set it to this now, so we calculate the time correctly
	target.y = ((Vertex*)GetPosition())->y;

	// if the two faces are not really perpendicular or if the spine is wider than
	// then we determine that we are on a "hip" and we just want to go across it without drifting left or right
	// so we want to project all our velocity straight up

	Vertex	horizontal_target_normal = target_normal;
	horizontal_target_normal.y = 0.0f;
	horizontal_target_normal.Normalize();

	Vertex cache_vel = *(Vertex*)GetVelocity();

	float face_dot = fabsf(D3DXVec3Dot((D3DXVECTOR3*)&skater_up, (D3DXVECTOR3*)&horizontal_target_normal));
	if (face_dot < 0.9f)
	{
		((Vertex*)GetVelocity())->y = ((Vertex*)GetVelocity())->Length();
		((Vertex*)GetVelocity())->x = 0.0f;
		((Vertex*)GetVelocity())->z = 0.0f;
	}
	else
	{
		// if spine more than two feet wide, then also don't allow drift
#define FEET(inch) ((inch)*12.0f)
		if (dist > FEET(2.0f))
		{
			((Vertex*)GetVelocity())->y = ((Vertex*)GetVelocity())->Length();
			((Vertex*)GetVelocity())->x = 0.0f;
			((Vertex*)GetVelocity())->z = 0.0f;
		}
	}

	// one inch out, to ensure miss the lip
	dist += 1.0f;

#if 0 // old transfer code
	// get angle to rotate about, being the vector perpendicular to the world up vector and the difference between the two face normals
	// (generally for a spine these normals will be opposite, however they might be up to 90 degrees or more off when doing a hip)

	Mth::Vector normal_diff = target_normal - skater_up;
	normal_diff[Y] = 0.0f;
	normal_diff.Normalize();

	m_spine_rotate_axis[X] = -normal_diff[Z];
	m_spine_rotate_axis[Y] = 0.0f;
	m_spine_rotate_axis[Z] = normal_diff[X];
	m_spine_rotate_axis[W] = 0.0f;;
#endif

	// for gravity calculations, temporarily pretend we are doing spine physics, so g is constant
	/*if (target.y > GetPosition()->y)
	  target.y = GetPosition()->y;*/
	time = CalculateDuration(target.y, ((Vertex*)GetPosition())->y, ((Vertex*)GetVelocity())->y, this);

	// subtract some frames of time, to ensure we make it
	// time -= m_frame_length * 2.0f;

	if (time < 0.1f)
	{
		time = 0.1f;
	}

	speed = dist / time;

	// if spine more than two foot wide, then make sure that we have enough speed to get over it
	// otherwise, just do a little pop over, and allow them to recover						  
	if (dist > 24.0f && speed * speed > ((Vertex*)GetVelocity())->LengthSqr() + 50.0f)
	{
		_printf("not enough speed\n");

		return false;
	}

	// we have found a target point, either by looking directly in front or by doing the drop-down method
	// but we don't want to go for it until there is a clear line from our current position to the target

	Vertex	target_XZ = target;
	target_XZ.y = GetPosition()->y;

	start = *GetPosition();
	end = target_XZ;
	SetRay(start, end);
	if (CollisionCheck(Collision::Flags::Hollow, Collision::IGNORE0))
	{
		_printf("too early\n");
		// don't do anything.  We have a valid transfer but we can wait until we get high enough before we try for it
		return true;
	}
	target_normal = *(Vertex*)&target_normal;
	// setup the transfer's matrix slerp

	//Vertex land_facing;
	if (!hip_transfer)
	{
		//target_normal = *(Vertex*)&(-target_normal);
		Slerp::facing = *(Vertex*)&(target - *(Vertex*)GetPosition());
		Slerp::facing.y = -(Slerp::facing.x * target_normal.x + Slerp::facing.z * target_normal.z) / target_normal.y;
		Slerp::facing.Normalize();
		/*land_facing = *(Vertex*)&(target - *(Vertex*)GetPosition());
		land_facing.y = -(land_facing.x * target_normal.x + land_facing.z * target_normal.z) / target_normal.y;
		land_facing.Normalize();*/
		_printf("land_facing X %f Y %f\n", Slerp::facing.x, Slerp::facing.y);
	}
	else
	{
		_printf("\nHIP TRANSFER\n");
		Vertex offset = *(Vertex*)&(target - *(Vertex*)GetPosition());
		offset.y = 0.0f;
		offset.Normalize();
		float dot = D3DXVec3Dot(&offset, &horizontal_target_normal);
		if (dot < 0.0f)
		{
			Slerp::facing = Vertex(0.0f, -1.0f, 0.0f);
			_printf("land_facing +\n");
		}
		else
		{
			Slerp::facing = Vertex(0.0f, -1.0f, 0.0f);
			_printf("land_facing -\n");
		}
	}

	Slerp::start = *(Matrix*)GetMatrix();
	*(D3DXVECTOR4*)Slerp::start.m[W] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

	// calculate the facing we want when we land; retain our horizontal direction and choose a vertical component which puts us parallel so the target
	// poly's plane

	// calculate goal matrix
	//Slerp::facing = *(Vertex*)&(-Slerp::facing);
	*(Vertex*)Slerp::end.m[Z] = Slerp::facing;
	(*(Vertex*)Slerp::end.m[Z]).ProjectToPlane(target_normal);
	(*(Vertex*)Slerp::end.m[Z]).Normalize();
	Slerp::end.m[Z][W] = 1.0f;
	*(Vertex*)Slerp::end.m[Y] = target_normal;
	Slerp::end.m[Y][W] = 1.0f;
	*(Vertex*)Slerp::end.m[X] = CrossProduct((Vertex*)Slerp::end.m[Y], (Vertex*)Slerp::end.m[Z]);
	Slerp::end.m[X][W] = 1.0f;
	*(D3DXVECTOR4*)Slerp::end.m[W] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

	// store the goal facing for use in adjusting the velocity at land time
	Slerp::goal = (*(Vertex*)Slerp::end.m[Z]);

	Matrix	inv;


	// Calculate the inverse transformation.
	inv = Slerp::start;
	inv.Invert();
	(D3DXMATRIX)inv = inv * Slerp::end;

	// Get the axis and angle.
	/*inv.GetRotationAxisAndAngle((Vertex*)&Slerp::axis, &Slerp::radians, start, end, hip_transfer);
	const float USE_LERP_INSTEAD_DEGREES = 2.0f;
	const float USE_LERP_INSTEAD_RADIANS = USE_LERP_INSTEAD_DEGREES * D3DX_PI / 180.0f;
	Slerp::lerp = Slerp::radians < USE_LERP_INSTEAD_RADIANS;*/



	//#if 0 // old transfer code
		// get angle to rotate about, being the vector perpendicular to the world up vector and the difference between the two face normals
		// (generally for a spine these normals will be opposite, however they might be up to 90 degrees or more off when doing a hip)
	/*target_normal = *(Vertex*)&(-target_normal);
	skater_up = *(Vertex*)&(-skater_up);*/
	//skater_up = *(Vertex*)&(-skater_up);
	Vertex normal_diff = *(Vertex*)&(target_normal - skater_up);
	//normal_diff[Y] = 0.0f;


	//normal_diff.Normalize();



	Slerp::axis[Y] = 0;// don't rotate Y since this bugs us..// -normal_diff[Y];

	/*if (fabsf(Slerp::axis[Z]) > (fabsf(Slerp::axis[X]) + 0.05f))
	{
		if (fabsf(Slerp::axis[X]) < 0.04f)
			Slerp::axis[X] = 0;
	}
	else if (fabsf(Slerp::axis[X]) > (fabsf(Slerp::axis[Z]) + 0.05f))
	{
		if (fabsf(Slerp::axis[Z]) < 0.04f)
			Slerp::axis[Z] = 0;
	}*/



	float xDiff = fabsf(start.x - end.x);
	if (fabsf(normal_diff[X]) > 0.005/*xDiff >= 5.0f*/)//x diff is big enough, so apply rotation on Z axis
		Slerp::axis[Z] = -normal_diff[X];
	else if (!hip_transfer)
		Slerp::axis[Z] = 0;
	float zDiff = fabsf(start.z - end.z);
	if (fabsf(normal_diff[Z]) > 0.005/*zDiff >= 5.0f*/)//z diff is big enough, so apply rotation on X axis
		Slerp::axis[X] = normal_diff[Z];
	else if (!hip_transfer)
		Slerp::axis[X] = 0;


	//If we have rotation on both X and Z it means we do a hip_transfer, or our velocity made us drift in the QP
	if (Slerp::axis[X] && Slerp::axis[Z]) [[unlikely]]
	{
		_printf("Both Axis...\n");
		if (hip_transfer) [[unlikely]]//hip_transfer, keeep both rotations
		{
			_printf("An unlikely event..\n");
		/*vTwoSinThetaAxis.x = m[UP][Z] - m[AT][Y];
		vTwoSinThetaAxis.z = m[UP][X] - m[RIGHT][Y];*/
			}
			else//need to check if we just drifted or if this is a non vertical ramp
			{
				_printf("drifted?");
				if (xDiff > zDiff)
				{
					if ((xDiff - zDiff) > 180.0f || fabsf(start.y - end.y) > 100)//we just drifted, so rotate ONLY on the longest distance
					{
						 Slerp::axis[X] = 0;
						 (*(Vertex*)&Slerp::axis).Normalize();
						 printf(" YES wall %f %f\n", wall_out.x, wall_out.z);
					}
					else
					{
						printf(" NO 1 wall %f %f\n", wall_out.x, wall_out.z);
						//vTwoSinThetaAxis.x = m[UP][Z] - m[AT][Y];
						//vTwoSinThetaAxis.y = m[AT][X] - m[RIGHT][Z];
						///vTwoSinThetaAxis.z = 0;// m[UP][X] - m[RIGHT][Y];
						//reverse = true;
						/*vTwoSinThetaAxis.x = m[UP][Z] - m[AT][Y];
						vTwoSinThetaAxis.y = m[AT][X] - m[RIGHT][Z];
						vTwoSinThetaAxis.z = m[RIGHT][Y] - m[UP][X];*/
					}
				 }
				 else
				 {
					 if ((zDiff - xDiff) > 180.0f || fabsf(start.y - end.y) > 100)//we just drifted, so rotate ONLY on the longest distance
					 {
						 Slerp::axis[Z] = 0;
						 (*(Vertex*)&Slerp::axis).Normalize();
						 printf(" YES wall %f %f\n", wall_out.x, wall_out.z);
					 }
					 else
							{
						 //reverse = true;
						 printf(" NO 2 wall %f %f\n", wall_out.x, wall_out.z);
						 /*vTwoSinThetaAxis.x = m[UP][Z] - m[AT][Y];
						 vTwoSinThetaAxis.y = m[AT][X] - m[RIGHT][Z];
						 vTwoSinThetaAxis.z = m[RIGHT][Y] - m[UP][X];*/
						 //vTwoSinThetaAxis.z = 0.5f;
						 //vTwoSinThetaAxis.z = 0.5f;
						 //vTwoSinThetaAxis.z = -m[RIGHT][Y] - m[UP][X];// *= -1.0f;// m[UP][X] - m[RIGHT][Y];
						 /*vTwoSinThetaAxis.x = 0;// m[UP][Z] - m[AT][Y];
						 //vTwoSinThetaAxis.y =  m[AT][X] - m[RIGHT][Z];
						 vTwoSinThetaAxis.z = 0;// m[UP][X] - m[RIGHT][Y];*/
					 }
				 }
			 }

	}
	else
		_printf("1 axis\n");
	/*if (fabsf(Slerp::axis[Z]) > (fabsf(Slerp::axis[X]) + 0.05f))
	{
		if (fabsf(Slerp::axis[X]) < 0.045f)
			Slerp::axis[X] = 0;
	}
	else if (fabsf(Slerp::axis[X]) > (fabsf(Slerp::axis[Z]) + 0.05f))
	{
		if (fabsf(Slerp::axis[Z]) < 0.045f)
			Slerp::axis[Z] = 0;
	}*/
	/*float nTwoSinTheta = (*(Vertex*)&(Slerp::axis)).Length();
	float nTwoCosTheta = inv.m[RIGHT][X] + inv.m[UP][Y] + inv.m[AT][Z] - 1.0f;
	Slerp::radians = (float)atan2(nTwoSinTheta, nTwoCosTheta);*/
	Slerp::radians = 3.0f;
	//Slerp::axis[Y] = 0;
	_printf("\nStart %f %f\nend %f %f\n", start.x, start.z, end.x, end.z);
	_printf("\nUp %f %f\ntarget_norm %f %f\n", skater_up.x, skater_up.z, target_normal.x, target_normal.z);
	_printf("Radian %f Axis %f %f\n", Slerp::radians, Slerp::axis.x, Slerp::axis.z);
	(*(Vertex*)&(Slerp::axis)).Normalize();
	_printf("Normalized Axis %f %f\n", Slerp::axis.x, Slerp::axis.z);
	_printf("diffY %f\n", normal_diff[Y]);
	normal_diff[Y] = 0.0f;
	normal_diff.Normalize();
	Matrix slerp_test;
	TestInterporlator(&slerp_test, 0.5f);

	if (slerp_test.m[Y][Y] < 0.0f)
	{
		_printf("Inverting Skater\n");

		Slerp::axis = -Slerp::axis;
		Slerp::radians = (2.0f * D3DX_PI) - Slerp::radians;
	}

	_printf("test_angle %f %f %f\n", slerp_test.m[Y][X], slerp_test.m[Y][Y], slerp_test.m[Y][Z]);
	if (!hip_transfer && Slerp::axis[X] && Slerp::axis[Z])//hip transfer seems to not be bugged?
	{

		//Here comes a hack to get the optimal angle
		//We check if skater is rotated to "stand" straight in the air @ 50% of transfer
		//If he is not standing straight enough even after this, we try to scale the axis
		float optimal = fabsf(slerp_test.m[Y][Y]);
		if (optimal < 0.95f)//standing straight enough?
		{
			int best = 1;
			Slerp::radians = 3.0f;
			D3DXVECTOR3 Optimal = Slerp::axis;
			_printf("goint to find optimum\n");
			D3DXMatrixIdentity(&slerp_test);
			Slerp::axis[X] = normal_diff[Z];
			Slerp::axis[Z] = -normal_diff[X];
			TestInterporlator(&slerp_test, 0.5f); //test 50 % of transfer
			if (slerp_test.m[Y][Y] > optimal)
			{
				optimal = slerp_test.m[Y][Y];
				Optimal = Slerp::axis;
				best++;
			}
			_printf("test_angle2 %f %f %f\n", slerp_test.m[Y][X], slerp_test.m[Y][Y], slerp_test.m[Y][Z]);

			D3DXMatrixIdentity(&slerp_test);
			Slerp::axis[X] = normal_diff[Z];
			Slerp::axis[Z] = normal_diff[X];
			TestInterporlator(&slerp_test, 0.5f);
			if (slerp_test.m[Y][Y] > optimal)
			{
				optimal = slerp_test.m[Y][Y];
				Optimal = Slerp::axis;
				best = 3;
			}
			_printf("test_angle3 %f %f %f\n", slerp_test.m[Y][X], slerp_test.m[Y][Y], slerp_test.m[Y][Z]);

			D3DXMatrixIdentity(&slerp_test);
			Slerp::axis[X] = -normal_diff[Z];
			Slerp::axis[Z] = -normal_diff[X];
			TestInterporlator(&slerp_test, 0.5f);
			if (slerp_test.m[Y][Y] > optimal)
			{
				optimal = slerp_test.m[Y][Y];
				Optimal = Slerp::axis;
				best = 4;
			}
			_printf("test_angle4 %f %f %f\n", slerp_test.m[Y][X], slerp_test.m[Y][Y], slerp_test.m[Y][Z]);

			D3DXMatrixIdentity(&slerp_test);
			Slerp::axis[X] = normal_diff[X];
			Slerp::axis[Z] = normal_diff[Z];
			TestInterporlator(&slerp_test, 0.5f);
			if (slerp_test.m[Y][Y] > optimal)
			{
				optimal = slerp_test.m[Y][Y];
				Optimal = Slerp::axis;
				best = 5;
			}
			_printf("test_angle5 %f %f %f\n", slerp_test.m[Y][X], slerp_test.m[Y][Y], slerp_test.m[Y][Z]);

			D3DXMatrixIdentity(&slerp_test);
			Slerp::axis[X] = -normal_diff[X];
			Slerp::axis[Z] = -normal_diff[Z];
			TestInterporlator(&slerp_test, 0.5f);
			if (slerp_test.m[Y][Y] > optimal)
			{
				optimal = slerp_test.m[Y][Y];
				Optimal = Slerp::axis;
				best = 6;
			}
			_printf("test_angle6 %f %f %f\n", slerp_test.m[Y][X], slerp_test.m[Y][Y], slerp_test.m[Y][Z]);

			D3DXMatrixIdentity(&slerp_test);
			Slerp::axis[X] = -normal_diff[X];
			Slerp::axis[Z] = normal_diff[Z];
			TestInterporlator(&slerp_test, 0.5f);
			if (slerp_test.m[Y][Y] > optimal)
			{
				optimal = slerp_test.m[Y][Y];
				Optimal = Slerp::axis;
				best = 7;
			}
			_printf("test_angle7 %f %f %f\n", slerp_test.m[Y][X], slerp_test.m[Y][Y], slerp_test.m[Y][Z]);

			D3DXMatrixIdentity(&slerp_test);
			Slerp::axis[X] = normal_diff[X];
			Slerp::axis[Z] = -normal_diff[Z];
			TestInterporlator(&slerp_test, 0.5f);
			if (slerp_test.m[Y][Y] > optimal)
			{
				optimal = slerp_test.m[Y][Y];
				Optimal = Slerp::axis;
				best = 8;
			}
			_printf("test_angle8 %f %f %f\n", slerp_test.m[Y][X], slerp_test.m[Y][Y], slerp_test.m[Y][Z]);

			Slerp::axis = Optimal;

			TestInterporlator(&slerp_test, 1.0f);
			float angDiffX = fabsf(slerp_test[Y][X] - target_normal[Z]);
			float angDiffZ = fabsf(slerp_test[Y][Z] - target_normal[X]);

			printf("angDiff %f %f\nNormalY %f\n", angDiffX, angDiffZ, target_normal[Y]);
			printf("final axis %f %f\nBestAngle %d\n", Slerp::axis[X], Slerp::axis[Z], best);


			TestInterporlator(&slerp_test, 0.5f);
			if (slerp_test.m[Y][Y] < 0.95f)//still not straight enough, let's try scaling...
			{
				_printf("Scaling axis: ");
				if (Slerp::axis[X] > Slerp::axis[Z])
				{
					_printf("X\n");
					Slerp::axis[X] /= 2;
					Slerp::axis[Z] += Slerp::axis[X];
				}
				else if (Slerp::axis[X] < Slerp::axis[Z])
				{
					_printf("Z\n");
					Slerp::axis[X] /= 2;
					Slerp::axis[Z] += Slerp::axis[X];
				}
				else
					_printf("Equal?\n");
			}
			/*TestInterporlator(&slerp_test, 0.5f);
			if (slerp_test.m[Y][Y] < 0.0f)
			{
				_printf("Inverting Skater\n");

				Slerp::axis = -Slerp::axis;
				Slerp::radians = (2.0f * D3DX_PI) - Slerp::radians;
			}*/
		}
	}


	//m_spine_rotate_axis[W] = 0.0f;;
//#endif

	// if the skater is entering the spine transfer with an odd facing due to rotation, we want to preserve that angle in the slerp's goal matrix

	// calculate the deviation between the skater's velocity and facing
	float angle = GetAngleAbout(*(Vertex*)&GetMatrix()->m[Z], cache_vel, *(Vertex*)&GetMatrix()->m[Y]);

	// be a bit forgiving for hip transfers, as you often have to hit left/right to trigger them, which causes rotation

	if (fabsf(angle) < D3DXToRadian(30.0f))
	{
		angle = 0.0f;
	}

	// rotate goal facing to reflect the deviation in the initial facing
	Slerp::end.RotateYLocal(-angle);

	// setup the slerp state
	Slerp::timer = 0.0f;
	Slerp::duration = ClampMin(time, 0.9f); // clamp the time to stop super fast rotations
	Slerp::old = Slerp::start;


	// remember the height we are aiming for, so when we come down through this height
	// then we remove the non vert velocity (or make it very small....)
	Slerp::height = target.y;

	// set velocity over the wall fast enough to land on the target point																	 
	Slerp::vel = *(Vertex*)&((target - *(Vertex*)GetPosition()) / time);		// velocity from start to target
	Slerp::vel.y = 0.0f;															// but ignore Y, as gravity handles that...

	// tell the code we are doing spine physics, so we lean quicker 
	if (!hip_transfer)
	{
		/*CStruct pParams;
		ExecuteQBScript("SkaterAwardTransfer",&pParams);*/	// award a trick (might want to do it as an exception later)
	}
	else
	{
		/*CStruct pParams;
		ExecuteQBScript("SkaterAwardHipTransfer", &pParams);*/	// award a trick (might want to do it as an exception later)
	}

	// no late jumps during a transfer
	//"ollied"....

	_printf("spinning target %f vel %f %f duration %f\n", target.z, Slerp::vel.x, Slerp::vel.z, Slerp::duration);

	Slerp::transfer = true;	// flag in spin physics, to do the lean forward, and also allow downcoming lip tricks
	Slerp::speed = GetSpeed((Vertex*)GetVelocity());
	_printf("\nVel %f %f %f Speed %f\n", GetVelocity()->x, GetVelocity()->y, GetVelocity()->z, Slerp::speed);
	inVert = true;
	static const DWORD timer = 0x00409AE0;
	_asm call timer
	static DWORD temp = 0;
	static DWORD temp2 = 0;
	_asm mov temp, eax
	_asm mov temp2, edx
	* (DWORD*)(&inVert + 8) = temp;
	*(DWORD*)(&inVert + 0xC) = temp2;
	tracking = false;	// we are still vert, but not tracking the vert
	canbreakvert = false;	// and as we "broke" vert, we don't want to do it again
	Slerp::done = false;
	Slerp::slerping = true;
	QBKeyHeader* header = GetQBKeyHeader(Checksums::Normal_Lerp_Speed);
	if (header)
	{

		Slerp::value = header->value.f;
		header->value.f = 0.01f;
	}

	/*Vector norm;
	norm.x = target_normal.x;
	norm.y = target_normal.y;
	norm.z = target_normal.z;
	norm.w = 1.0f;
	new_normal(norm);*/

	return true;
}


bool TestForClearPath(Vertex& target, Vertex& vel, Vertex& pos, Skater* skater)
{
	float initial_horiz_speed = sqrtf(vel.x * vel.x + vel.z * vel.z);

	Vertex drop_direction = vel;
	drop_direction.y = 0.0f;
	float length = drop_direction.Length();
	if (length < 0.01f) return false;
	drop_direction *= 1.0f / length;
	Vertex horizontal_offset = *(Vertex*)&(target - pos);
	horizontal_offset.y = 0.0f;
	float distance = horizontal_offset.Length();
	float original_target_height = target.y;
	if (D3DXVec3Dot(&horizontal_offset, &drop_direction) < 0.0f)
	{
		distance = -distance;
	}
	drop_direction = *(Vertex*)&(horizontal_offset / distance);

	Vertex hold_vel = vel;
	vel[Y] = fminf(vel[Y], 2.0f * 200.0f);
	//give a slight upward pop
   /*if (skated_off_edge)
   {
	   vel[Y] = fmaxf(vel[Y], 200.0f);
   }*/


   // grab the acceleration we will have during our acid drop
	float acceleration = skater->GetAirGravity();

	// calculate what height we would have if we used our current horizontal velocity to reach the target position
	float final_height;
	/*if (distance > 0.0f && initial_horiz_speed > 0.0001f)
	{*/
	float time_to_target = distance / initial_horiz_speed;
	final_height = pos.y + vel.y * time_to_target + 0.5f * acceleration * time_to_target * time_to_target;
	/*}
	else
	{
	// for backwards acid drops, just act as through we are directly over the target point
	final_height = pos.y;
	}*/
	final_height += 2.0f;
	/*if (final_height > pos.y)
	  final_height = pos.y;*/

	  // if we can't reach the target with our current velocity, ditch the acid drop
	if (final_height < target.y)
	{
		//MessageBox(0, "3", "", 0);
		return false;
	}

	// calculate the air time before the acid drop would hit its true target; prevent acid drops from occuring moments before landing
	float time_to_reach_target_height = CalculateDuration(original_target_height, pos.y, vel.y, skater);
	/*if (time_to_reach_target_height < 0.5f)
	{
	  //MessageBox(0, "2", "", 0);
	  return false;
	}*/

	// ensure that we have a clear shot to the target

	bool clear_path = false;

	// keep shifting our target point up until we can get a clear shot to it, or we get to an unreachable height
	while (target.y < final_height)
	{
		Vertex cstart = pos;

		// check a path constructed from two concatenated lines, with the midpoint halfway along the acid drop trajectory; this is an attempt
		// to allow most acid drop which might be disallowed by a ledge which would block a straight line

		// calculate the time span required to fall to the target height
		float half_time_to_reach_target_height = 0.5f * CalculateDuration(target.y, pos.y, vel.y, skater);

		// calculate the spine velocity which would be used for this target
		float required_speed = 0.5f * distance / half_time_to_reach_target_height;

		// calculate the height we will be at halfway through the acid drop
		float height_halfway = pos.y + vel.y * half_time_to_reach_target_height
			+ 0.5f * acceleration * half_time_to_reach_target_height * half_time_to_reach_target_height;//sqrtf(half_time_to_reach_target_height);

		  // calculate the point halfway through the acid drop
		Vertex halfway_point = pos;
		halfway_point.y = height_halfway;
		halfway_point += drop_direction * required_speed * half_time_to_reach_target_height;

		// check for collisions alone the two-line path
		Vertex cend = halfway_point;
		skater->SetRay(*(D3DXVECTOR3*)&cstart, *(D3DXVECTOR3*)&cend);
		if (!skater->CollisionCheck(Collision::Flags::Hollow, Collision::IGNORE0))
		{
			// feeler.DebugLine(255, 255, 0);
			cstart = cend;
			cend = target;
			cend.y += 2.0f;
			skater->SetRay(*(D3DXVECTOR3*)&cstart, *(D3DXVECTOR3*)&cend);
			if (!skater->CollisionCheck(Collision::Flags::Hollow, Collision::IGNORE0))
			{
				// feeler.DebugLine(255, 255, 0);
				return true;
			}
			else
			{
				// MessageBox(0, "hited", "", 0);
				 // feeler.DebugLine(0, 0, 0, 0);
			}
		}

		// feeler.DebugLine(0, 0, 0, 0);

		// try a higher target point
		target.y += 24.0f;
	}

	return false;
}

bool maybe_acid(bool skated_off_edge, const Vertex& pos, const Vertex& old_pos, Vertex& vel, SAcidDropData& acid_drop_data)
{

	Skater* skater = Skater::GetSkater();
	// horizontal direction in which a drop would occur
	Vertex drop_direction = vel;
	drop_direction.y = 0.0f;
	float length = drop_direction.Length();
	if (length < 0.01f) return false;
	drop_direction *= 1.0f / length;

	bool target_found = false;
	Vertex target;

	// in order not to miss vert polys with a thin horizontal projection, we check for them starting at this frame's initial position
	Vertex search_pos = old_pos;
	search_pos.y = fmaxf(pos.y, old_pos.y);

	float scan_distance = 500.0f;
	float scan_height = 0.0f;


	Vertex target_normal;
	Vertex horizontal_target_normal;
	float distance;
	for (distance = 0.01f; distance < scan_distance; distance += 3.0f)
	{
		// look for a vert poly below us
		D3DXVECTOR3 cstart = (drop_direction * distance + search_pos);
		D3DXVECTOR3 cend = cstart;
		cstart.y += scan_height;
		cend.y -= 4000.0f;
		skater->SetRay(cstart, cend);
		if (skater->CollisionCheck(Collision::Flags::Vert) && is_vert_for_transfers((Vertex*)skater->GetCollisionNormal()))
		{
			// the horizontal projection of the vert's normal just correspond somewhat to our direction			 
			target_normal = *(Vertex*)skater->GetCollisionNormal();
			*(Vertex*)&(horizontal_target_normal) = target_normal;
			horizontal_target_normal.y = 0.0f;
			horizontal_target_normal.Normalize();


			target_found = D3DXVec3Dot(&drop_direction, &horizontal_target_normal) >= 0.05f;

			if (target_found)
			{
				target = *(Vertex*)skater->GetHitPoint();
				// feeler.m_end[Y] += 3960.0f;
				// feeler.DebugLine(255, 100, 100, 0);
				break;
			}
			else
			{
				// feeler.m_end[Y] += 3960.0f;
				// feeler.DebugLine(100, 255, 100, 0);
			}
		}
		else
		{
			// feeler.m_end[Y] += 3960.0f;
			// feeler.DebugLine(100, 100, 255, 0);
		}

		// use a larger incrememt at larger distances, as we have several frames yet to find these polys
		if (distance > 100.0f)
		{
			distance += 24.0f;
		}
	}

	if (!target_found)
	{
		// no valid acid drop target found
		return false;
	}

	float original_target_height = target.y;

	// because our search began behind us, the horizontal offset to the target may not be forward
	Vertex horizontal_offset = *(Vertex*)&(target - pos);
	horizontal_offset.y = 0.0f;
	distance = horizontal_offset.Length();
	if (D3DXVec3Dot((D3DXVECTOR3*)&horizontal_offset, (D3DXVECTOR3*)&drop_direction) < 0.0f)
	{
		distance = -distance;
	}
	drop_direction = *(Vertex*)&(horizontal_offset / distance);

	// stash a copy of velocity so we can pretend it has an adjusted value
	Vertex hold_vel = vel;

	// calculate our effective horizontal velocity
	float initial_horiz_speed = sqrtf(vel.x * vel.x + vel.z * vel.z);

	// give a slight upward pop
	if (skated_off_edge)
	{
		vel[Y] = fmaxf(vel[Y], 200.0f);
	}

	// grab the acceleration we will have during our acid drop
	float acceleration = skater->GetAirGravity();

	// calculate what height we would have if we used our current horizontal velocity to reach the target position
	float final_height;
	if (distance > 0.0f && initial_horiz_speed > 0.0001f)
	{
		float time_to_target = distance / initial_horiz_speed;
		final_height = pos.y + vel.y * time_to_target + 0.5f * acceleration * time_to_target * time_to_target;
	}
	else
	{
		// for backwards acid drops, just act as through we are directly over the target point
		final_height = pos.y;
	}

	// if we can't reach the target with our current velocity, ditch the acid drop
	if (final_height < target.y)
	{
		vel = hold_vel;
		return false;
	}

	// calculate the air time before the acid drop would hit its true target; prevent acid drops from occuring moments before landing
	if (original_target_height > ((Vertex*)skater->GetPosition())->y)
		original_target_height = ((Vertex*)skater->GetPosition())->y;
	float time_to_reach_target_height = CalculateDuration(original_target_height, pos.y, vel.y, skater);
	if (time_to_reach_target_height < 1.0f)
	{
		vel = hold_vel;
		return false;
	}

	// ensure that we have a clear shot to the target

	bool clear_path = false;

	// keep shifting our target point up until we can get a clear shot to it, or we get to an unreachable height
	while (target.y < final_height)
	{
		Vertex cstart = pos;

		// check a path constructed from two concatenated lines, with the midpoint halfway along the acid drop trajectory; this is an attempt
		// to allow most acid drop which might be disallowed by a ledge which would block a straight line

		// calculate the time span required to fall to the target height
		if (target.y > ((Vertex*)skater->GetPosition())->y)
			target.y = ((Vertex*)skater->GetPosition())->y;
		float half_time_to_reach_target_height = 0.5f * CalculateDuration(target.y, pos.y, vel.y, skater);

		// calculate the spine velocity which would be used for this target
		float required_speed = 0.5f * distance / half_time_to_reach_target_height;

		// calculate the height we will be at halfway through the acid drop
		float height_halfway = pos.y + vel.y * half_time_to_reach_target_height
			+ 0.5f * acceleration * half_time_to_reach_target_height * half_time_to_reach_target_height;// sqrtf(half_time_to_reach_target_height);

		  // calculate the point halfway through the acid drop
		Vertex halfway_point = pos;
		halfway_point.y = height_halfway;
		halfway_point += drop_direction * required_speed * half_time_to_reach_target_height;

		// check for collisions alone the two-line path
		Vertex cend = halfway_point;
		skater->SetRay(*(D3DXVECTOR3*)&cstart, *(D3DXVECTOR3*)&cend);
		if (!skater->CollisionCheck(Collision::Flags::Hollow, Collision::IGNORE0))
		{
			// feeler.DebugLine(255, 255, 0);
			cstart = cend;
			cend = target;
			cend.y += 1.0f;
			skater->SetRay(*(D3DXVECTOR3*)&cstart, *(D3DXVECTOR3*)&cend);
			if (!skater->CollisionCheck(Collision::Flags::Hollow, Collision::IGNORE0))
			{
				// feeler.DebugLine(255, 255, 0);
				clear_path = true;
				break;
			}
			else
			{
				// feeler.DebugLine(0, 0, 0, 0);
			}
		}

		// feeler.DebugLine(0, 0, 0, 0);

		// try a higher target point
		target.y += 24.0f;
	}

	// no clear path was found along the acid drop
	if (!clear_path)
	{
		vel = hold_vel;
		return false;
	}

	acid_drop_data.target_pos = target;
	acid_drop_data.target_normal = target_normal;
	acid_drop_data.true_target_height = original_target_height;
	return true;
}


void EnterAcid(const SAcidDropData& data)
{
	DEBUGSTART()
	{
		Skater* skater = Skater::GetSkater();
		_printf("EnterAcid\n");

		//Slerp::done = false;
		//oldMatrix = this->matrix;
		//MessageBox(0, "EnterAcid", "", 0);
		const Vertex& target_pos = data.target_pos;
		const Vertex& target_normal = data.target_normal;
		const float& true_target_height = data.true_target_height;

		Vertex horizoffset = *(Vertex*)&(target_pos - *(Vertex*)skater->GetPosition());
		horizoffset.y = 0.0f;
		float distance = horizoffset.Length();

		if (D3DXVec3Dot((D3DXVECTOR3*)&horizoffset, (D3DXVECTOR3*)skater->GetVelocity()) < 0.0f)
			distance = -distance;
		//distance += 2.0f;

		D3DXVECTOR3 dropdirr = horizoffset / distance;

		//Slerp::transfer = true;
		/*float posY = target_pos.y;
		if (target_pos.y > this->GetPosition()->y)
		  MessageBox(0, "WTF", "", 0);
		if (target_pos.y > GetPosition()->y)
		  posY = GetPosition()->y;*/
		if (target_pos.y > ((Vertex*)skater->GetPosition())->y)
			Slerp::duration = CalculateDuration(true_target_height, (skater->GetPosition())->y, (skater->GetVelocity())->y, skater);
		else
			Slerp::duration = CalculateDuration(target_pos.y, (skater->GetPosition())->y, (skater->GetVelocity())->y, skater);
		_printf("duration %f target %f vel %f\n", Slerp::duration, target_pos.y, (skater->GetVelocity())->y);

		//Slerp::transfer = false;
		float reqspeed = distance / Slerp::duration;

		printf("reqspeed %f dropdirr %f %f\n", reqspeed, dropdirr.x, dropdirr.z);
		Slerp::vel = Vertex(reqspeed * dropdirr.x, 0.0f, reqspeed * dropdirr.z);
		//Slerp::vel.y = 0.0f;
		Slerp::height = target_pos.y;
		printf("vel %f %f %f %f\n", Slerp::vel.x, Slerp::vel.z, reqspeed * dropdirr.x, reqspeed * dropdirr.z);

		//MessageBox(0, "VertTrue?", "", 0);
		skater->SetVertAir(true);
		//MessageBox(0, "Ya", "", 0);
		Slerp::transfer = true;
		Slerp::speed = GetSpeed((Vertex*)skater->GetVelocity());
		_printf("\nVelocity %f %f %f Speed %f\n", skater->GetVelocity()->x, skater->GetVelocity()->y, skater->GetVelocity()->z, Slerp::speed);
		//MessageBox(0, "Ya2", "", 0);

		((Vertex*)skater->GetVelocity())->x = 0.0f;
		((Vertex*)skater->GetVelocity())->z = 0.0f;

		Slerp::start = *(Matrix*)skater->GetMatrix();
		/*Slerp::facing = dropdirr;
		Slerp::facing.y = -(Slerp::facing.x * Slerp::facing.x + Slerp::facing.z * Slerp::facing.z) / target_normal.y;
		Slerp::facing.Normalize();*/
		Slerp::facing = *(Vertex*)&(target_pos - *(Vertex*)skater->GetPosition());
		Slerp::facing.y = -(Slerp::facing.x * target_normal.x + Slerp::facing.z * target_normal.z) / target_normal.y;
		Slerp::facing.Normalize();
		//Slerp::facing = -Slerp::facing;

		*(Vertex*)Slerp::end.m[Z] = Slerp::facing;
		(*(Vertex*)Slerp::end.m[Z]).ProjectToPlane(target_normal);
		(*(Vertex*)Slerp::end.m[Z]).Normalize();
		Slerp::end.m[Z][W] = 1.0f;
		*(Vertex*)Slerp::end.m[Y] = target_normal;
		Slerp::end.m[Y][W] = 1.0f;
		*(Vertex*)Slerp::end.m[X] = CrossProduct((Vertex*)Slerp::end.m[Y], (Vertex*)Slerp::end.m[Z]);
		Slerp::end.m[X][W] = 1.0f;
		*(D3DXVECTOR4*)Slerp::end.m[W] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

		Slerp::goal = (*(Vertex*)&(Slerp::end.m[Z]));

		Matrix	inv;


		// Calculate the inverse transformation.
		inv = Slerp::start;
		inv.Invert();
		(D3DXMATRIX)inv = inv * Slerp::end;

		// Get the axis and angle.
		inv.GetRotationAxisAndAngle((Vertex*)&Slerp::axis, &Slerp::radians, *skater->GetPosition(), *(Vertex*)&target_pos);
		const float USE_LERP_INSTEAD_DEGREES = 2.0f;
		const float USE_LERP_INSTEAD_RADIANS = USE_LERP_INSTEAD_DEGREES * D3DX_PI / 180.0f;
		Slerp::lerp = Slerp::radians < USE_LERP_INSTEAD_RADIANS;

		Vertex horizfacing = *(Vertex*)&(skater->GetMatrix()->m[Z]);
		horizfacing.y = 0.0f;
		float angle = GetAngleAbout(horizfacing, *(Vertex*)&dropdirr, *(Vertex*)&(skater->GetMatrix()[Y]));
		Slerp::end.RotateYLocal(-angle);
		Slerp::timer = 0.0f;
		Slerp::old = Slerp::start;


		QBKeyHeader* header = GetQBKeyHeader(Checksums::Normal_Lerp_Speed);
		if (header)
		{

			Slerp::value = header->value.f;
			header->value.f = 0.01f;
		}

		Vector norm;
		norm.x = target_normal.x;
		norm.y = target_normal.y;
		norm.z = target_normal.z;
		norm.w = 1.0f;
		skater->SetNormal(norm);
		//MessageBox(0, "done acid", "", 0);
		Slerp::slerping = false;
		Slerp::done = false;
	}
	DEBUGEND()
}

bool TestForAcid(CStruct* pParams, CScript* pScript)
{
	if (Slerp::inAcid || Slerp::transfer || Slerp::landing)
		return false;
	Slerp::landed = false;
	SAcidDropData acid_drop_data;
	Skater* skater = Skater::GetSkater();
	skater->Store();
	if (LevelModSettings::AllowNewTricks & LevelModSettings::ALLOW_SPINE && skater->InVert())
	{
		if (skater->CheckForSpine())
		{
			//found a spine target
			Slerp::type = SPINE;
			Slerp::vert = true;
			//skater->Restore();
			return Slerp::transfer;
		}
		else
		{
			Slerp::landing = true;
			skater->Restore();
			Vertex forward = *(Vertex*)&(*skater->GetVelocity());
			if (forward.y < 0.0f)
			{
				return false;
			}

			/*if (skater->IsTracking())
			{
				skater->SetTracking(false);
				//skater->SetCanBreakVert(false);
			}*/
			//skater->ResetLerpingMatrix();

			Slerp::last = *(Vertex*)skater->GetVelocity();
			/*skater->SetVertAir(false);*/
			/*skater->SetCanBreakVert(false);
			skater->SetLanded(false);*/
			Slerp::vert = false;
			Slerp::done = true;


			/*D3DXVECTOR3 start = *skater->GetPosition();
			D3DXVECTOR3 end = start;
			end.y -= 4000;
			skater->SetRay(start, end);
			skater->CollisionCheck(0x10);
			skater->GetVelocity()->x = 0;
			skater->GetVelocity()->z = 0;

			Slerp::start = *(Matrix*)skater->GetMatrix();
			Vertex target_pos = *(Vertex*)skater->GetHitPoint();
			Vertex target_normal = *(Vertex*)skater->GetCollisionNormal();
			Slerp::facing = *(Vertex*)&(target_pos - *(Vertex*)skater->GetPosition());
			Slerp::facing.y = -(Slerp::facing.x * target_normal.x + Slerp::facing.z * target_normal.z) / target_normal.y;
			Slerp::facing.Normalize();
			//Slerp::facing = -Slerp::facing;

			*(Vertex*)Slerp::end.m[Z] = Slerp::facing;
			(*(Vertex*)Slerp::end.m[Z]).ProjectToPlane(target_normal);
			(*(Vertex*)Slerp::end.m[Z]).Normalize();
			Slerp::end.m[Z][W] = 1.0f;
			*(Vertex*)Slerp::end.m[Y] = target_normal;
			Slerp::end.m[Y][W] = 1.0f;
			*(Vertex*)Slerp::end.m[X] = CrossProduct((Vertex*)Slerp::end.m[Y], (Vertex*)Slerp::end.m[Z]);
			Slerp::end.m[X][W] = 1.0f;
			*(D3DXVECTOR4*)Slerp::end.m[W] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

			Slerp::goal = (*(Vertex*)&(Slerp::end.m[Z]));*/

			Slerp::duration = 0;
			Slerp::timer = 0;

			Slerp::transfer = false;
			Slerp::slerping = false;

			CStruct pStruct;
			CScript pScript;


			
			
			

			if (skater->IsTracking())
			{
				pStruct.head = NULL;
				pStruct.tail = NULL;
				skater->CallMemberFunction(Checksum("resetlandedfromvert"), &pStruct, &pScript);
				
				/*skater->SetVertAir(false);
				skater->SetCanBreakVert(false);*/
				//skater->ResetLerpingMatrix();



				CStructHeader param(QBKeyHeader::INT, 500);
				pStruct.AddParam(&param);
				skater->CallMemberFunction(Checksum("SetSpeed"), &pStruct, &pScript);

				/*skater->GetVelocity()->x *= 0.1f;
				skater->GetVelocity()->z *= 0.1f;*/
				D3DXVECTOR3 vel = *skater->GetVelocity();
				skater->GetVelocity()->x = 0.0f;
				skater->GetVelocity()->y = 0.0f;
				skater->GetVelocity()->z = 0.0f;

				param.Type = QBKeyHeader::LOCAL;
				param.Data = Checksum("Ground");
				skater->CallMemberFunction(Checksum("SetState"), &pStruct, &pScript);

				skater->GetVelocity()->x *= 0.1f;
				skater->GetVelocity()->z *= 0.1f;
				

				
				

				*skater->GetVelocity() = vel;

				skater->GetVelocity()->x *= 0.1f;
				skater->GetVelocity()->z *= 0.1f;

				param.QBkey = Checksum("y");
				param.value.i = -20;
				skater->CallMemberFunction(Checksum("Move"), &pStruct, &pScript);
				pStruct.head = NULL;
				pStruct.tail = NULL;
				skater->SetTracking(false);

				param.Type = QBKeyHeader::LOCAL;
				param.QBkey = 0;
				param.Data = Checksum("Air");
				skater->CallMemberFunction(Checksum("SetState"), &pStruct, &pScript);
				/*param.Type = QBKeyHeader::FLOAT;
				param.QBkey = Checksum("z");
				param.value.f = 90.0f;
				float value = 0.5f;
				CStructHeader param2(QBKeyHeader::FLOAT, Checksum("duration"), *(DWORD*)&value);
				CStructHeader param3(QBKeyHeader::LOCAL, Checksum("seconds"));
				param.NextHeader = &param2;
				param2.NextHeader = &param3;
				pStruct.tail = &param3;*/
				
				skater->CallMemberFunction(Checksums::OrientToNormal, &pStruct, &pScript);

			}
			else
			{
				CStructHeader param(QBKeyHeader::INT, 500);
				pStruct.AddParam(&param);
				skater->CallMemberFunction(Checksum("SetSpeed"), &pStruct, &pScript);
				pStruct.head = NULL;
				pStruct.tail = NULL;
				skater->CallMemberFunction(Checksums::OrientToNormal, &pStruct, &pScript);
			}


			/*

			Slerp::transfer = true;
			Slerp::slerping = true;
			_printf("No Spine Target...\n");
			skater->Restore();
			Slerp::last = *(Vertex*)skater->GetVelocity();
			skater->SetVertAir(false);
			skater->SetCanBreakVert(false);	// and as we "broke" vert, we don't want to do it again
			skater->SetLanded(false);
			Slerp::vert = false;
			Slerp::done = true;
			D3DXVECTOR3 start = *skater->GetPosition();
			D3DXVECTOR3 end = start;
			end.y -= 4000;
			skater->SetRay(start, end);
			skater->CollisionCheck(0x10);
			skater->GetVelocity()->x = 0;
			skater->GetVelocity()->z = 0;

			Slerp::start = *(Matrix*)skater->GetMatrix();
			Vertex target_pos = *(Vertex*)skater->GetHitPoint();
			Vertex target_normal = *(Vertex*)skater->GetCollisionNormal();
			Slerp::facing = *(Vertex*)&(target_pos - *(Vertex*)skater->GetPosition());
			Slerp::facing.y = -(Slerp::facing.x * target_normal.x + Slerp::facing.z * target_normal.z) / target_normal.y;
			Slerp::facing.Normalize();
			//Slerp::facing = -Slerp::facing;

			*(Vertex*)Slerp::end.m[Z] = Slerp::facing;
			(*(Vertex*)Slerp::end.m[Z]).ProjectToPlane(target_normal);
			(*(Vertex*)Slerp::end.m[Z]).Normalize();
			Slerp::end.m[Z][W] = 1.0f;
			*(Vertex*)Slerp::end.m[Y] = target_normal;
			Slerp::end.m[Y][W] = 1.0f;
			*(Vertex*)Slerp::end.m[X] = CrossProduct((Vertex*)Slerp::end.m[Y], (Vertex*)Slerp::end.m[Z]);
			Slerp::end.m[X][W] = 1.0f;
			*(D3DXVECTOR4*)Slerp::end.m[W] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

			Slerp::goal = (*(Vertex*)&(Slerp::end.m[Z]));
			Slerp::duration = 10.0f;

			Matrix	inv;


			// Calculate the inverse transformation.
			inv = Slerp::start;
			inv.Invert();
			(D3DXMATRIX)inv = inv * Slerp::end;

			// Get the axis and angle.
			//inv.GetRotationAxisAndAngle((Vertex*)&Slerp::axis, &Slerp::radians);
			//const float USE_LERP_INSTEAD_DEGREES = 2.0f;
			//const float USE_LERP_INSTEAD_RADIANS = USE_LERP_INSTEAD_DEGREES * D3DX_PI / 180.0f;
			//Slerp::lerp = Slerp::radians < USE_LERP_INSTEAD_RADIANS;

			Vertex horizfacing = *(Vertex*)&(skater->GetMatrix()->m[Z]);
			horizfacing.y = 0.0f;

			Vertex horizoffset = *(Vertex*)&(target_pos - *(Vertex*)skater->GetPosition());
			horizoffset.y = 0.0f;
			float distance = horizoffset.Length();

			if (D3DXVec3Dot((D3DXVECTOR3*)&horizoffset, (D3DXVECTOR3*)skater->GetVelocity()) < 0.0f)
				distance = -distance;
			//distance += 2.0f;

			D3DXVECTOR3 dropdirr = horizoffset / distance;

			float angle = GetAngleAbout(horizfacing, *(Vertex*)&dropdirr, *(Vertex*)&(skater->GetMatrix()[Y]));
			Slerp::end.RotateYLocal(-angle);
			Slerp::timer = 0.0f;
			Slerp::old = Slerp::start;


			QBKeyHeader* header = GetQBKeyHeader(Checksums::Normal_Lerp_Speed);
			if (header)
			{

				Slerp::value = header->value.f;
				header->value.f = 0.01f;
			}

			Vector norm;
			norm.x = target_normal.x;
			norm.y = target_normal.y;
			norm.z = target_normal.z;
			norm.w = 1.0f;
			//skater->SetNormal(norm);
			*/
			return false;
		}
	}
	else
	{
		if (LevelModSettings::AllowNewTricks & LevelModSettings::ALLOW_ACID)
		{
			Vertex pos = *(Vertex*)skater->GetOldPosition();
			pos.x = max(pos.x, skater->GetPosition()->x);
			Vertex start = pos;
			Vertex end = start;

			Vertex velocity = *(Vertex*)skater->GetVelocity();


			Vertex vel_normal;// = GetNormal(&velocity);

			vel_normal = velocity;
			vel_normal.y = 0.0f;
			float length = vel_normal.Length();
			if (length < 0.01f) return false;
			vel_normal *= 1.0f / length;

			for (float dist = 0.01f; dist < 550.0f; dist += 3.0f)
			{


				start.x = pos.x + vel_normal.x * dist;
				start.z = pos.z + vel_normal.z * dist;

				end.x = pos.x + vel_normal.x * dist;
				end.y = pos.y - 4000.0f;
				end.z = pos.z + vel_normal.z * dist;
				//velocity.y -= 0.5f;
				skater->SetRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end);


				if (skater->CollisionCheck(Collision::Flags::Vert) && is_vert_for_transfers((Vertex*)skater->GetCollisionNormal()))
				{
					Vertex target_normal = *(Vertex*)skater->GetCollisionNormal();
					Vertex vel = *(Vertex*)skater->GetVelocity();
					Vertex target = *(Vertex*)skater->GetHitPoint();
					Vertex truetarget = target;
					//_printf("above\n");


					/*if (!TestForClearPath(target, vel, pos, skater))
					  continue;*/
					  //
					float height = ((Vertex*)skater->GetPosition())->y - truetarget.y;

					if (height < 20.0f)
						continue;

					/*float time = skater->CalculateDuration(skater->GetHitPoint()->y, skater->GetPosition()->y, skater->GetVelocity()->y);

					// subtract some frames of time, to ensure we make it
					// time -= m_frame_length * 2.0f;

					if (time < 0.1f)
					{
					  time = 0.1f;
					}

					float speed = dist / time;

					// if spine more than two foot wide, then make sure that we have enough speed to get over it
					// otherwise, just do a little pop over, and allow them to recover
					if (dist > 24.0f && speed * speed > skater->GetVelocity()->LengthSqr())
					{
					  _printf("not enough speed\n");
					  continue;
					}*/

					Vertex horizontal_target_normal = target_normal;
					horizontal_target_normal.y = 0.0f;
					horizontal_target_normal.Normalize();

					if (D3DXVec3Dot((D3DXVECTOR3*)&vel_normal, (D3DXVECTOR3*)&horizontal_target_normal) >= 0.05f)
					{
						/*Vertex target_normal = *skater->GetCollisionNormal();
						Vertex vel = *skater->GetVelocity();
						Vertex target = *skater->GetHitPoint();
						Vertex truetarget = target;
						_printf("above\n");
						fflush(stdout);*/

						if (TestForClearPath(target, vel, pos, skater))
						{
							_printf("above\n");
							skater->Restore();
							Slerp::inAcid = true;
							Slerp::type = ACID;
							//skater->SetNormals();
							//skater->GetVelocity()->y = velocity.y;
							SAcidDropData acid_drop_data;
							acid_drop_data.target_pos = target;
							//acid_drop_data.target_pos.y += 2.0f;
							acid_drop_data.target_normal = target_normal;

							acid_drop_data.true_target_height = truetarget.y;
							Slerp::vert = true;
							*skater->GetVelocity() = vel;
							EnterAcid(acid_drop_data);
							Slerp::inAcid = false;
							return true;
						}
						else
							_printf("no clear path?\n");
					}
				}

				if (dist > 120.0f)
				{
					dist += 24.0f;
				}
			}
		}
		/*Vertex vel = *skater->GetVelocity();
		if (skater->maybe_acid_drop(false, *skater->GetPosition(), *skater->GetOldPosition(), *skater->GetVelocity(), acid_drop_data))
		{
		  Slerp::type = ACID;
		  Slerp::inAcid = true;
		  Slerp::vert = true;
		  skater->EnterAcid(acid_drop_data);
		  Slerp::inAcid = false;
		  return true;
		}*/
		//*skater->GetVelocity() = vel;
		if (LevelModSettings::AllowNewTricks & LevelModSettings::ALLOW_BANK)
		{
			skater->Restore();
			int num_hits = 0;

			skater->Store();

			Vertex pos = *(Vertex*)skater->GetOldPosition();
			Vertex start = pos;
			Vertex end = start;

			Vertex velocity = *(Vertex*)skater->GetVelocity();


			for (float dist = 0.01f; dist < 550.0f; dist += 3.0f)
			{
				Vertex vel_normal = GetNormal(&velocity);
				vel_normal.y = 0.0f;

				start.x = pos.x + vel_normal.x * dist;
				start.z = pos.z + vel_normal.z * dist;

				end.x = pos.x + vel_normal.x * dist;
				end.y = pos.y - 4000.0f;
				end.z = pos.z + vel_normal.z * dist;
				velocity.y -= 0.5f;
				skater->SetRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end);


				if (skater->CollisionCheck(Collision::Flags::Hollow, Collision::IGNORE0))
				{
					Vertex target_normal = *(Vertex*)skater->GetCollisionNormal();
					Vertex vel = *(Vertex*)skater->GetVelocity();
					Vertex target = *(Vertex*)skater->GetHitPoint();
					Vertex truetarget = target;
					//_printf("above\n");


					/*if (!TestForClearPath(target, vel, pos, skater))
					  continue;*/
					float height = ((Vertex*)skater->GetPosition())->y - truetarget.y;

					if (height < 20.0f)
						continue;


					float absnorm = fabsf(target_normal.y);
					if (absnorm > 0.48f && absnorm < 0.98f)
					{
						//_printf("norm %f %f vel %f %f", vel_normal.x, vel_normal.z, velocity.x, velocity.z);
						float test = ((vel_normal.x - target_normal.x) + (vel_normal.z - target_normal.z));
						if (fabsf(test) < 0.82f)
						{
							_printf("above\n");

							/*num_hits++;

							if (num_hits == 3)
							{*/

							/*Vertex target_normal = *skater->GetCollisionNormal();
							Vertex vel = *skater->GetVelocity();
							Vertex target = *skater->GetHitPoint();
							Vertex truetarget = target;
							_printf("above\n");
							fflush(stdout);*/

							if (TestForClearPath(target, vel, pos, skater))
							{

								skater->Restore();
								Slerp::inAcid = true;
								Slerp::type = BANK;
								//skater->SetNormals();
								//skater->GetVelocity()->y = velocity.y;
								SAcidDropData acid_drop_data;
								acid_drop_data.target_pos = target;
								//acid_drop_data.target_pos.y += 2.0f;
								acid_drop_data.target_normal = target_normal;

								acid_drop_data.true_target_height = truetarget.y;
								Slerp::vert = false;
								EnterAcid(acid_drop_data);
								Slerp::inAcid = false;
								return true;
							}
							else
								_printf("no clear path?\n");

							/*skater->Restore();
							Slerp::inAcid = true;
							Slerp::type = BANK;
							//skater->SetNormals();
							//skater->GetVelocity()->y = velocity.y;
							SAcidDropData acid_drop_data;
							acid_drop_data.target_pos = *skater->GetHitPoint();
							acid_drop_data.target_pos.y += 1.0f;
							acid_drop_data.target_normal = *skater->GetCollisionNormal();

							acid_drop_data.true_target_height = acid_drop_data.target_pos.y;
							Slerp::vert = false;
							skater->EnterAcid(acid_drop_data);
							Slerp::inAcid = false;
							return true;*/
							//}

							if (num_hits > 3)
							{
							}

							/*pos = *skater->GetPosition();
							Vertex hit = *skater->GetHitPoint();
							float oldY = pos.y;
							Vertex transport;
							transport.x = hit.x - pos.x;
							transport.z = hit.x - pos.z;

							pos.x += transport.x*0.2f;
							pos.z += transport.z*0.2f;
							skater->SetPosition(pos);*/


						}
						else
							num_hits = 0;
					}
				}

				if (dist > 150.0f)
				{
					dist += 24.0f;
				}
			}
		}
	}
	Slerp::inAcid = false;
	Slerp::transfer = false;
	Slerp::landing = false;
	skater->Restore();
	return false;
}


void MaybeAcid()
{
	if (Slerp::wallplant)
	{
		Skater* skater = Skater::GetSkater();
		if ((_GetCurrentTime() - Slerp::m_last_wallplant_time_stamp) > Physics_Wallplant_Duration)
		{
			_printf("Wallplant wait time done - Applying velocity\n");
			*skater->GetVelocity() = Slerp::realVelocity;
			Slerp::wallplant = false;
		}
		return;
	}

	if (!(LevelModSettings::AllowNewTricks & LevelModSettings::ALL_SPINE) || Slerp::inAcid || Slerp::transfer || Slerp::landing)
		return;

	SAcidDropData acid_drop_data;
	Skater* skater = Skater::GetSkater();
	if (skater == NULL) [[unlikely]]
		return;
	skater->Store();
	if (!Slerp::OnGround && skater->GetKeyState(LevelModSettings::SpineButton)->IsPressed() && (LevelModSettings::SpineButton2 == KeyState::NONE || skater->GetKeyState(LevelModSettings::SpineButton2)->IsPressed()))
	{
		_printf("trying acid\n");

		/*if (skater->maybe_acid_drop(false, *skater->GetPosition(), *skater->GetOldPosition(), *skater->GetVelocity(), acid_drop_data))
		  skater->EnterAcid(acid_drop_data);
		  else
		  {*/
		if (TestForAcid(NULL, NULL))
		{
			_printf("acid returned true\n");

			Slerp::trying = true;
			Slerp::done = false;
			skater->CallMemberFunction(Checksum("DoNextTrick"));
			switch (Slerp::type)
			{
			case ACID:
				skater->FlagException("AcidDrop");
				break;
			case BANK:
				skater->FlagException("BankDrop");
				break;
			case SPINE:
				skater->FlagException("SpineTransfer");
				break;
			}
		}
		//}
	}
	skater->Restore();
	Slerp::OnGround = false;
	Slerp::OnGrind = false;
}

bool Skater::CheckForWallpant()
{
	if (!(LevelModSettings::AllowNewTricks & LevelModSettings::ALLOW_WALLPLANT))
		return false;
	//Check that we are standing "up"
	_printf("%f\n", GetMatrix()[Y][Y]);
	//if(GetMatrix()[Y][Y] < -0.90f) return false;

	DWORD currentTime = _GetCurrentTime();
	if ((currentTime - Slerp::m_last_wallplant_time_stamp) < Physics_Disallow_Rewallplant_Duration) return false;

	Slerp::m_last_wallplant_time_stamp = currentTime;
	_printf("Timer passed, height %f!\n", height);

	if (height < Physics_Min_Wallplant_Height) return false;

	//Check for vertpoly flag and that the wall is straight enough
	if (!(GetCollFlags() & 0x8) && fabsf(GetCollisionNormal()->y) > 0.1f) return false;

	Vertex* velocity = (Vertex*)GetVelocity();

	float speed = velocity->Length();
	_printf("Speed %f\n", speed);
	if (speed < 0.01f) return false;

	Vertex forward = *(Vertex*)&(*velocity / speed);

	Vertex horiz_forward = forward;
	horiz_forward.y = 0.0f;
	horiz_forward.Normalize();

	Vertex wall_normal = *(Vertex*)GetCollisionNormal();
	Vertex horiz_normal = wall_normal;
	horiz_normal.y = 0;
	horiz_normal.Normalize();

	_printf("Angle %f min aproach %f\n", D3DXVec3Dot(&horiz_forward, &horiz_normal), -sinf(Physics_Wallplant_Min_Approach_Angle * D3DX_PI / 180.0f));

	if (D3DXVec3Dot(&horiz_forward, &horiz_normal) > -sinf(Physics_Wallplant_Min_Approach_Angle * 180 / D3DX_PI)) return false;

	_printf("Angle passed\n");

	if (velocity->y > 0.0f && GetKeyState(KeyState::GRIND)->IsPressed()) [[unlikely]]
	{
		D3DXVECTOR3 wall_point = *GetHitPoint();

		Vertex wall_up_vel(0.0f, velocity->y * 0.15f, 0.0f);
		wall_up_vel.RotateToPlane(wall_normal);

		wall_point += wall_up_vel;

		Store();
		SetRay(wall_point + wall_normal * 6.0f, wall_point - wall_normal * 6.0f);
		//Ignore noncollidable
		if (!CollisionCheck(Collision::Flags::Hollow, Collision::IGNORE0))
			return false;
		Restore();
	}

	CArray* p_trick_query_array = GetQBKeyHeader(Checksum("Wallplant_Trick"))->pArray;
	if (p_trick_query_array == NULL)
	{
		_printf("couldn't find array Wallplant_Trick\n");
		return false;
	}
	bool triggered = false;
	for (DWORD i = 0; i < p_trick_query_array->GetNumItems(); ++i)
	{
		CStruct* p_trick_query_struct = p_trick_query_array->GetStructure(i);
		if (QueryEvents(p_trick_query_struct))
		{
			triggered = true;
			break;
		}
	}
	if (!triggered)
		return false;
	velocity->y = 0.0f;

	D3DXVECTOR3 perp_vel = D3DXVec3Dot(velocity, &horiz_normal) * horiz_normal;
	*velocity -= 2.0f * perp_vel;

	float horizontal_speed = sqrtf(velocity->x * velocity->x + velocity->z * velocity->z);
	if (horizontal_speed > 0.0001f)
	{
		velocity->y = 0.0f;
		velocity->Normalize(fmaxf(Physics_Wallplant_Min_Exit_Speed, (horizontal_speed - Physics_Wallplant_Speed_Loss)));
	}
	else
	{
		*velocity = *(Vertex*)&(Physics_Wallplant_Min_Exit_Speed * horiz_normal);
	}

	velocity->y = Physics_Wallplant_Vertical_Exit_Speed;

	// move to just outside the wall, insuring that there is no additional collision along the line to that point
	SetRay(*GetHitPoint(), *GetHitPoint() + Physics_Wallplant_Distance_From_Wall * wall_normal);

	if (CollisionCheck(Collision::Flags::Hollow, Collision::IGNORE0))
	{
		*GetPosition() = *GetHitPoint() + 0.1f * wall_normal;
	}
	else
	{
		*GetPosition() = endcol;
	}

	/**(Vertex*)&GetMatrix()[Z] = *velocity;
	GetMatrix()[Z][Y] = 0.0f;
	((Vertex*)&GetMatrix()[Z])->Normalize();
	GetMatrix()[Y][X] = 0.0f;
	GetMatrix()[Y][Y] = 1.0f;
	GetMatrix()[Y][Z] = 0.0f;
	D3DXVec3Cross((D3DXVECTOR3*)&GetMatrix()[X], (D3DXVECTOR3*)&GetMatrix()[Y], (D3DXVECTOR3*)&GetMatrix()[Z]);*/
	//ResetLerpingMatrix();

	Slerp::realVelocity = *velocity;
	CScript pScript;
	CStruct pStruct;
	velocity->y = 0.0f;
	//CallMemberFunction(Checksum("DoNextTrick"));

	CallMemberFunction(Checksum("TurnToFaceVelocity"), &pStruct, &pScript);
	FlagException("Wallplant");



	velocity->x = 0.0f;
	velocity->z = 0.0f;
	Slerp::wallplant = true;
	blockspin = true;
	autoturn = false;
	_printf("returning true\n");
	return true;
}


__declspec(naked) void airhook_naked()
{
	static Skater* skater;
	static DWORD oldEBP;
	static DWORD jmpBack = 0x004A0F10;
	static DWORD CheckForWallride = 0x004A32C0;

	_asm mov skater, ecx;
	_asm pushad;

	if (skater->CheckForWallpant())
	{
		_asm popad;
		_asm add esp, 4;
		_asm jmp[jmpBack];
	}
	else
	{
		_asm popad;
		_asm jmp[CheckForWallride];
	}
}

__declspec(naked) void MaybeAcid_naked()
{
	static const DWORD jmpBack = 0x004A8AE0 + 4;
	_asm pushad;
	/*_printf("inside maybeacidd\n");
	fflush(stdout);*/
	MaybeAcid();
	_asm popad;
	static const DWORD handle_air = 0x004A25A0;
	_asm call handle_air
	_asm jmp[jmpBack];
}

__declspec(naked) void CheckForTransfer_naked()
{
	static Skater* skater = NULL;
	_asm mov skater, ebp
	_asm pushad;
	static const DWORD jmpBack = 0x004A2DAB + 4;
	static bool ok = false;
	//_asm pop retaddr;

	if (Slerp::transfer == false || Slerp::slerping == false)
	{
		ok = false;
		static DWORD adjustnormal = 0x0049EAC0;
		//static DWORD preadjust = 0x0049BA80;
		//D3DXVECTOR4* norml = skater->GetCurrentNormal();
		//DWORD param = (DWORD)skater + 0x000086B8;
		//_asm push param
		_asm mov ecx, skater
		//_asm call preadjust
		_asm call adjustnormal;
		//_asm mov ok, al;
	}
	else
	{
		ok = true;
		//_asm pushad;
		//_asm pushfd;
		//_printf("inside checkfortransfer\n");
		//skater->ResetLerpingMatrix();
		//_asm popfd;
		//_asm popad
	}
	if (Slerp::type == SPINE && Slerp::transfer)
		ok = true;
	//_asm push retaddr;
	_asm popad;
	_asm mov al, ok
	_asm jmp[jmpBack]
}

__declspec(naked) void ResetTransferVel_naked()
{
	static Vertex* velocity = NULL;
	_asm mov velocity, ebx
	static const DWORD jmpBack = 0x004A27B5 + 4;
	_asm pushad;
	//_asm pushfd;
	//_printf("inside resettrasnferver\n");
	//_asm popfd;
	if (Slerp::transfer && Slerp::addedvel && velocity)
	{
		//_printf("Reset velocity %f %f\nvel %f %f\n", velocity->x, velocity->z, Slerp::vel.x, Slerp::vel.z);
		*velocity -= Slerp::vel;
	}
	_asm popad;
	static DWORD vrtx = 0x0048A720;
	_asm call vrtx;
	_asm jmp[jmpBack];
}

__declspec(naked) void Slerp_naked()
{
	/* _asm pushad;
	 _asm pushfd;*/
	static Skater* skater = NULL;
	_asm mov skater, ebp
	static const DWORD jmpBack = 0x004A25D9 + 4;
	//_asm pushfd;
	//_printf("inside slerp\n");
	if (!Slerp::transfer)// && skater->InVert())
	{
		static DWORD lean = 0x0049CB60;
		_asm call lean
	}
	else
	{
		_asm pushad;
		_asm pushfd;
		if (skater)
			skater->Slerping();
		_asm popfd;
		_asm popad;
	}
	//_asm popfd;
	/*_asm popfd;
	_asm popad;*/
	_asm jmp[jmpBack];
}



void OnGround()
{
	Slerp::OnGround = true;
	Slerp::OnGrind = false;
	Slerp::landing = false;
	Slerp::slerping = false;
	Slerp::wallplant = false;

	if (Slerp::transfer) [[unlikely]]
	{
		Slerp::transfer = false;

		Skater* skater = Skater::GetSkater();
		if (skater == NULL)
			return;
		_printf("landed from transfer 2\n");
		//fflush(stdout);
		QBKeyHeader* header = GetQBKeyHeader(Checksums::Normal_Lerp_Speed);
		if (header)
			header->value.f = Slerp::value;


		skater->ResetLerpingMatrix();
		if (Slerp::landed == false)
		{

			/**skater->GetVelocity() = Slerp::last;
			skater->GetVelocity()->RotateToNormal(Slerp::goal);
			skater->RotateToPlane();
			Vertex test_vel = *skater->GetVelocity();
			skater->remove_sideways_velocity(&test_vel);
			if (test_vel.y > 0.0f)
			{
			  // use a standard landing instead
			  *skater->GetVelocity() = Slerp::last;
			  skater->GetVelocity()->ProjectToPlane(*skater->GetNormal());
			}

			float minSpeed = 5000.0f;
			if (skater->GetVelocity()->LengthSqr() < minSpeed*minSpeed)
			{
			  skater->GetVelocity()->Normalize(minSpeed);
			}*/
			skater->SetLanded(Slerp::vert);
			Slerp::landed = true;
			/* MessageBox(0, "", "", 0);
			 //*skater->GetVelocity() = -*skater->GetVelocity();

			 CStruct CSpeed;
			 CStructHeader hSpeed;
			 hSpeed.Type = QBKeyHeader::QBKeyType::FLOAT;
			 hSpeed.QBkey = Checksum("Speed");
			 hSpeed.value.f = Slerp::speed;
			 CSpeed.head = &hSpeed;
			 CSpeed.tail = &hSpeed;
			 ExecuteQBScript("SetSpeedScripts", &CSpeed);
			 QBKeyHeader* header = GetQBKeyHeader(Checksum("SetSpeed"));



			 _printf("Set Speed%p  Vel %f %f %f\n", header, skater->GetVelocity()->x, skater->GetVelocity()->y, skater->GetVelocity()->z);*/
		}
	}
}


__declspec(naked) void OnGround_naked()
{
	static const DWORD jmpBack = 0x004A21AF + 4;
	_asm pushad;
	_asm pushfd;
	//_printf("inside onground\n");
	OnGround();
	_asm popfd;
	_asm popad;
	static DWORD unk1 = 0x0040FA90;
	_asm call unk1
	_asm jmp[jmpBack];
}


DWORD GrindParamHook(char* str, int unk)
{
	//_printf("OnGrind?\n");
	Slerp::OnGrind = true;
	Slerp::wallplant = false;
	if ((Slerp::transfer || Slerp::landing)) [[unlikely]]
	{
		Slerp::OnGround = false;
		Skater* skater = Skater::GetSkater();
		if (skater)
		{
			_printf("OnGrind while inside transfer\nReseting transfer...\n");
			QBKeyHeader* header = GetQBKeyHeader(Checksums::Normal_Lerp_Speed);
			if (header)
				header->value.f = Slerp::value;

			//skater->SetVertAir(false);
			skater->SetLanded(false);

			Slerp::vert = false;
			Slerp::transfer = false;
			Slerp::landing = false;
			Slerp::slerping = false;
			skater->ResetLerpingMatrix();
			Slerp::landed = true;

		}
	}
	typedef DWORD(__cdecl* const pGrindParam)(char*, int);
	return pGrindParam(0x00426570)(str, unk);
}

void FixSpineFlag()
{
	//MessageBox(0, "Fixing Spine", "", 0);
	DWORD old;
	VirtualProtect((void*)0x004A2DAA, 9, PAGE_EXECUTE_READWRITE, &old);
	//*(BYTE*)0x004A2DAA = 0xE9;
	DWORD addr = 0x004A2DAA;
	*(BYTE*)addr = 0xE9;
	addr++;
	DWORD offset = (PtrToUlong(CheckForTransfer_naked) - 0x004A2DAA) - 5;
	*(DWORD*)addr = offset;
	addr += 4;
	*(DWORD*)addr = 0x3F74C084;


	VirtualProtect((void*)0x004A25D8, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004A25D8 = 0xE9;
	addr = 0x004A25D9;
	offset = (PtrToUlong(Slerp_naked) - 0x004A25D8) - 5;
	*(DWORD*)addr = offset;

	VirtualProtect((void*)0x004A3096, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004A3096 = 0xE9;
	addr = 0x004A3097;
	offset = (PtrToUlong(ResetTransfer_naked) - 0x004A3096) - 5;
	*(DWORD*)addr = offset;

	VirtualProtect((void*)0x004A27B4, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004A27B4 = 0xE9;
	addr = 0x004A27B5;
	offset = (PtrToUlong(ResetTransferVel_naked) - 0x004A27B4) - 5;
	*(DWORD*)addr = offset;



	VirtualProtect((void*)0x004A21AE, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004A21AE = 0xE9;
	addr = 0x004A21AF;
	offset = (PtrToUlong(OnGround_naked) - 0x004A21AE) - 5;
	*(DWORD*)addr = offset;


	VirtualProtect((void*)0x004294C6, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x0004294C6 = 0xE8;
	addr = 0x004294C7;

	VirtualProtect((void*)0x004A0D19, 4, PAGE_EXECUTE_READWRITE, &old);
	addr = 0x004A0D19;
	offset = (PtrToUlong(airhook_naked) - 0x004A0D19) - 4;
	*(DWORD*)addr = offset;


	/*Hooked_AddCStruct = (Hooked_AddCStruct)CStruct::MaybeAddCompressed;*/
	/*typedef BYTE* (__thiscall* CStruct::* pFunc)(DWORD, BYTE*, QBKeyInfoContainer*);
	pFunc p = (pFunc)&CStruct::MaybeAddCompressed;*/


	//BYTE* (__thiscall CStruct:: * pFunc)(DWORD, BYTE*, QBKeyInfoContainer*) = &CStruct::MaybeAddCompressed; <-this
	//void (MyClass:: * fptr) (void) = &MyClass::fun;


	//sprintf(msg, "%X %X", &pFunc, pFunc);
	//*(DWORD*)addr = (DWORD)(void*&)pFunc - 0x4294CB; <-this
	//MessageBox(0, msg, msg, 0);


	VirtualProtect((void*)0x004A8ADF, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004A8ADF = 0xE9;
	addr = 0x004A8AE0;
	offset = (PtrToUlong(MaybeAcid_naked) - 0x004A8ADF) - 5;
	*(DWORD*)addr = offset;


	/*VirtualProtect((void*)0x00403149, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x00403149 = 0xE9;
	addr = 0x0040314A;
	offset = (PtrToUlong(Fopen_naked) - 0x00403149) - 5;
	*(DWORD*)addr = offset;*/
	/*if (debugMode)
	{*/
	DWORD fopen = *(DWORD*)0x0058D0B0;
	VirtualProtect((void*)fopen, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)fopen = 0xE9;
	addr = fopen + 1;
	offset = (PtrToUlong(Fopen_naked) - fopen) - 5;
	*(DWORD*)addr = offset;
	//}
	/*VirtualProtect((void*)0x0058D0B0, 4, PAGE_EXECUTE_READWRITE, &old);
	 = (DWORD)_fopen;*/


	extern bool debugMode;
	if (debugMode)
	{
		VirtualProtect((void*)0x004265F0, 5, PAGE_EXECUTE_READWRITE, &old);
		*(BYTE*)0x004265F0 = 0xE9;
		addr = 0x004265F1;
		offset = (PtrToUlong(Checksum_naked) - 0x004265F0) - 5;
	}
	char msg[128] = "";
	/*sprintf(msg, "OFFSET %X", offset);
	MessageBox(0, msg, msg, 0);*/
	*(DWORD*)addr = offset;


	VirtualProtect((void*)0x004B2B58, 4, PAGE_EXECUTE_READWRITE, &old);

	addr = 0x004B2B58;
	offset = (PtrToUlong(GrindParamHook) - 0x004B2B58) - 4;
	*(DWORD*)addr = offset;

}


void Interporlate(Matrix* result, float delta)
{
	if (delta <= 0.0f)
	{
		// delta = 0.0f;
		*result = Slerp::start;
		return;
	}
	else if (delta >= 1.0f)
	{
		delta = 1.0f;
		/**result = Slerp::end;
		return;*/
	}

	if (Slerp::lerp && false)
	{
		/* Get the lerp matrix */
		Matrix	lerp;
		Vertex	lpos;
		Vertex	spos;
		Vertex	epos;
		Vertex	rpos;

		D3DXMatrixIdentity(&lerp);

		spos = *(Vertex*)&Slerp::start.m[POS];
		epos = *(Vertex*)&Slerp::end.m[POS];

		*(D3DXVECTOR4*)&lerp[RIGHT] = *(D3DXVECTOR4*)&Slerp::end[RIGHT] - *(D3DXVECTOR4*)&Slerp::start[RIGHT];
		*(D3DXVECTOR4*)&lerp[UP] = *(D3DXVECTOR4*)&Slerp::end[UP] - *(D3DXVECTOR4*)&Slerp::start[UP];
		*(D3DXVECTOR4*)&lerp[AT] = *(D3DXVECTOR4*)&Slerp::end[AT] - *(D3DXVECTOR4*)&Slerp::start[AT];
		lpos = *(Vertex*)&(epos - spos);

		/* Do lerp */
		(*(Vector*)&(lerp[RIGHT])).Scale(delta);
		(*(Vector*)&(lerp[UP])).Scale(delta);
		(*(Vector*)&(lerp[AT])).Scale(delta);
		lpos.Scale(delta);

		*(D3DXVECTOR4*)&(*result)[RIGHT] = *(D3DXVECTOR4*)&Slerp::start[RIGHT] + *(D3DXVECTOR4*)&lerp[RIGHT];
		*(D3DXVECTOR4*)&(*result)[UP] = *(D3DXVECTOR4*)&Slerp::start[UP] + *(D3DXVECTOR4*)&lerp[UP];
		*(D3DXVECTOR4*)&(*result)[AT] = *(D3DXVECTOR4*)&Slerp::start[AT] + *(D3DXVECTOR4*)&lerp[AT];
		rpos = *(Vertex*)&(spos + lpos);

		(*(Vertex*)&((*result)[RIGHT])).Normalize();
		(*(Vertex*)&((*result)[UP])).Normalize();
		(*(Vertex*)&((*result)[AT])).Normalize();

		/**(D3DXVECTOR4*)&(*result).m[POS] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		*(Vertex*)&(*result)[POS] = rpos;*/
	}
	else
	{
		Matrix	lerp;
		D3DXVECTOR4	lpos;
		D3DXVECTOR4	spos;
		D3DXVECTOR4	epos;
		D3DXVECTOR4	rpos;

		D3DXMatrixIdentity(&lerp);
		spos = *(D3DXVECTOR4*)&Slerp::start.m[POS];
		epos = *(D3DXVECTOR4*)&Slerp::end.m[POS];
		*result = Slerp::start;

		//*(D3DXVECTOR4*)&(*result).m[POS] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

		result->Rotate(Slerp::axis, Slerp::radians * delta);
		lpos = epos - spos;
		(*(Vector*)&lpos).Scale(delta);
		rpos = spos + lpos;

		//*(D3DXVECTOR3*)&(*result).m[POS] = *(D3DXVECTOR3*)&rpos;
	}
}

float	SmoothStep(float value)
{
	// interpolates from zero to one with a zero derivative at the end-points
	return -2.0f * value * value * (value - (3.0f / 2.0f));
}


float Skater::GetAirGravity()
{
	float gravity = -1350.0f / this->GetScriptedStat("Physics_vert_hang_Stat");
	/*if (CheatIsOn(GetCheat(crc32f((unsigned char*)"CHEAT_MOON"))))
		gravity *= 0.5f;*/
	return gravity;
}


float CalculateDuration(float target_height, float pos_Y, float vel_Y, Skater* skater)
{
	// s = ut - 1/2 * g * t^2			 (note, -g = a in the more traditional formula)
	// solve this using the quadratic equation, gives us the formula below
	// Note the sign of s is important.....
	float distance = pos_Y - target_height;
	float vel = vel_Y;
	float acceleration = -skater->GetAirGravity();

	return (vel + sqrtf(vel * vel + 2.0f * acceleration * distance)) / acceleration;
}
