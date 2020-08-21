#undef NO_DEFINES
#include "pch.h"
#include "Skater.h"
#include "Physics.h"

Vertex scol, ecol;

bool Modulating()
{
	SkaterProfileContainer skater;
	if (skater.profile)
	{
		Settings* settings = skater.profile->GetSettings();
		if (settings)
			return settings->IsTrue("should_modulate_color");
	}
	return false;
}

typedef bool(__thiscall* const pCollisionCheck)(Skater* pThis, BYTE ignore, BYTE flags, BYTE ignore2);
EXTERN bool Skater::CollisionCheck(BYTE flag, bool ignore)
{
	if (ignore)
		return pCollisionCheck(0x0049FC80)(this, flag, 0, 0);
	else
		return pCollisionCheck(0x0049FC80)(this, 0, flag, 0);
}


EXTERN void Skater::SetRay(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	this->startcol = start;
	this->endcol = end;


	/*lineVertices[0].x = start.x;
	lineVertices[0].y = start.y;
	lineVertices[0].z = start.z;
	lineVertices[0].rhw = 1.0f;
	lineVertices[0].colour = D3DCOLOR_XRGB(255, 0, 0);
	lineVertices[1].x = end.x;
	lineVertices[1].y = end.y;
	lineVertices[1].z = end.z;
	lineVertices[1].rhw = 1.0f;
	lineVertices[1].colour = D3DCOLOR_XRGB(255, 0, 0);
	numLineVertices = 2;*/

	/*f_AddRay AddRayd = (f_AddRay)GetProcAddress(GetModuleHandle("d3d8.dll"), "AddRay");
	if (AddRayd == NULL)
	MessageBox(0, "WTF?", "", 0);
	AddRayd(start, end, c);*/
}


EXTERN void Skater::SetCanBreakVert(bool value)
{
	canbreakvert = value;
}


EXTERN void Skater::SetVertAir(bool value)
{
	inVert = value;
	tracking = false;
	//canbreakvert = false;
	autoturn = false;
	static const DWORD timer = 0x00409AE0;
	_asm call timer
	static DWORD temp = 0;
	static DWORD temp2 = 0;
	_asm mov temp, eax
	_asm mov temp2, edx

	* (DWORD*)(&inVert + 8) = temp;
	*(DWORD*)(&inVert + 0xC) = temp2;


}


EXTERN void Skater::MultiplyMatrix(const D3DXMATRIX& delta)
{
	matrix *= delta;
	old = matrix;
	lerp = matrix;

}

void Skater::Slerping()
{
	//_printf("Slerping\n");
	//MessageBox(0, "trying to slerp", "", 0);

	//KeyState* state = this->GetKeyState(KeyState::UP);

	Slerp::timer += this->GetFrameLength();

	/*if (Slerp::timer > 0.05f && Slerp::done == false)
	{
	Matrix slerp;
	Interporlate(&slerp, 0.5f);
	Matrix inv = Slerp::old;
	inv.InvertUniform();
	Matrix delta = *(Matrix*)&(*(D3DXMATRIX*)&inv**(D3DXMATRIX*)&slerp);
	this->MultiplyMatrix(delta);
	if (!Slerp::trying)
	Slerp::done = true;
	Slerp::trying = false;
	}*/


	/**(Vertex*)delta.m[X] = Vertex(0.0f, 0.0f, 0.0f);
	delta.m[Y][W]  = 1.0f;
	*(Vertex*)delta.m[Z] = Vertex(0.0f, 0.0f, 0.0f);
	delta.m[W][X] = 0.0f;
	delta.m[W][Y] = 0.0f;
	delta.m[W][Z] = 0.0f;
	delta.m[W][W] = 1.0f;*/
	/*delta.m[W][W] = 1.0f;
	delta.m[X][W] = 0.0f;
	delta.m[Y][W] = 0.0f;
	delta.m[Z][W] = 1.0f;*/
	/*if (test == 0)
	  test = 1;
	  else if (test == 1)
	  test = 2;
	  else if (test == 2)
	  {*/
	if (Slerp::slerping)
	{
		if (Slerp::timer <= Slerp::duration * 1.0f)
		{
			Matrix slerp;
			Interporlate(&slerp, SmoothStep(ClampMax(Slerp::timer / Slerp::duration, 1.0f)));
			Matrix inv = Slerp::old;
			inv.InvertUniform();
			Matrix delta = *(Matrix*)&(*(D3DXMATRIX*)&inv * *(D3DXMATRIX*)&slerp);
			this->MultiplyMatrix(delta);
			Slerp::old = slerp;
		}
		else
		{
			_printf("FINAL\n");
			Slerp::slerping = false;
			//tracking = true;
			/*QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"Normal_Lerp_Speed"));
			if (header)
			{

			  Slerp::value = header->value.f;
			  header->value.f = 0.01f;
			}*/
			if (Slerp::duration < 0.9f)
			{
				Vector norm = *(Vector*)&Slerp::end.m[Y];
				printf("norm %f %f %f, end %f %f %f", norm.x, norm.y, norm.z, Slerp::end.m[Y][X], Slerp::end.m[Y][Y], Slerp::end.m[Y][Z]);

				SetNormal(norm);
			}
			else
			{
				/*Vector norm = *(Vector*)&Slerp::end.m[Y];
				SetNormal(norm);*/
				/*CStruct pStruct;
				CScript pScript;
				Skater* skater = Skater::GetSkater();
				skater->CallMemberFunction(Checksums::OrientToNormal, &pStruct, &pScript);*/
			}
			//SetPosition(*GetHitPoint());
		}
	}




	/*if (Slerp::timer < 0.08f)
	{
	/*Matrix slerp;
	D3DXMatrixIdentity(&slerp);
	slerp.Rotate(Slerp::axis, 3.0f*Slerp::timer);
	Matrix inv = Slerp::old;
	inv.InvertUniform();
	Matrix delta = *(Matrix*)&(*(D3DXMATRIX*)&inv**(D3DXMATRIX*)&slerp);
	this->MultiplyMatrix(delta);
	Slerp::trying = false;
	Slerp::old = slerp;/
	}
	else if (Slerp::done==false)
	{
	Matrix inv = Slerp::old;
	inv.InvertUniform();
	Matrix delta = *(Matrix*)&(*(D3DXMATRIX*)&inv**(D3DXMATRIX*)&Slerp::end);
	this->MultiplyMatrix(delta);
	Slerp::done = true;
	Slerp::trying = false;
	Slerp::old = Slerp::end;
	}*/


	/*if (Slerp::timer >= Slerp::duration)
	  matrix *= Slerp::end;
	  else
	  this->GetMatrix().Rotate(Slerp::axis, Slerp::timer/Slerp::duration);*/
	  /*if (GetMatrix().m[Y][Y] > -0.05f)
		this->GetMatrix().Rotate(Slerp::axis, -3.0f*framelength);
	  test = 0;
	  //}
	  /*_asm mov ecx, this
	  static DWORD adjustnormal = 0x0049EAC0;
	  _asm call adjustnormal*/
	Slerp::addedvel = true;
	Vertex* v = (Vertex*)this->GetVelocity();
	Vertex* p = (Vertex*)this->GetPosition();
	if (v->y < 0.0f && p->y < Slerp::height)	// just check if we have dropped below the target height		
	{
		Slerp::vel.Normalize(0.1f); // make spine velocity very small, but still there, so camera works
		Slerp::addedvel = false;
		/*if (Slerp::timer >= Slerp::duration)
		{
		  ResetLerpingMatrix();
		  Slerp::transfer = false;
		  Slerp::slerping = false;
		  SetLanded(Slerp::landed);
		}*/
	}
	else
		*v += Slerp::vel;
	Slerp::last = *v;
}


void Skater::Store()
{
	/*skpos = position;
	skopos = oldpos;
	skvel = velocity;*/
	oldCollNormal = normal;
	oldHitPoint = hitpoint;
	oldCollFlag = collFlags;
	oldHeight = height;
	scol = *(Vertex*)&startcol;
	ecol = *(Vertex*)&endcol;
}

void Skater::Restore()
{
	/*position = skpos;
	oldpos = skopos;
	skvel = velocity;^*/
	normal = oldCollNormal;
	hitpoint = oldHitPoint;
	collFlags = oldCollFlag;
	height = oldHeight;
	scol = *(Vertex*)&startcol;
	ecol = *(Vertex*)&endcol;
}

void Skater::ResetLerpingMatrix()
{
	old = matrix;
	currentnormal = *(Vector*)&GetMatrix()[Y];
	displaynormal = *(Vector*)&GetMatrix()[Y];
	lastdisplaynormal = *(Vector*)&GetMatrix()[Y];
	normallerp = 0.0f;
	//old = matrix;
}