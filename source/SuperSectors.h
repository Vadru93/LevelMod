#pragma once
#ifndef SUPERSECTOR_H
#define SUPERSECTOR_H
#undef NO_DEFINES
#include "Checksum.h"
#include "Mesh.h"
#include "Node.h"


//Collision stuff 00501CE0
struct MovingObject;
extern void RemoveMovingObject(SuperSector* sector);
extern bool updatingObjects;


EXTERN struct SuperSector
{
	DWORD FFFFFFFF;
	WORD* indices;
	D3DXVECTOR3* vertices;
	DWORD* pUnk1;//always NULL?
	DWORD* pUnk2;//Looks like axis for CollisionTree?
	DWORD* pUnk3;//Huge data...
	DWORD* pUnk4;//Looks like colors?
	Object* object;
	DWORD* pUnk6[6];//Points to itself
	D3DXVECTOR3 bboxMax;
	D3DXVECTOR3 bboxMin;
	DWORD* pUnk7;//Always NULL?
	Mesh* mesh;
	DWORD* pUnk9;//Always NULL?
	WORD padding;
	WORD numVertices;
	WORD numIndices;
	WORD unk1;//maybe flags?
	DWORD* pUnk10;//maybe this is the leaf??
	DWORD* pUnk11;//Always NULL?
	DWORD flag;//always 6?
	DWORD* pUnk12;//Always NULL?
	WORD* pCollisionFlags;//the flags for skatable, trigger etc
	DWORD* pUnk13;//bunch of 0xFF and random data
	DWORD* pUnk14;//bunch of 00
	BYTE unknown2;//Used when collision checking
	BYTE padding2[3];
	DWORD unknownChecksum;// Checksum??
	DWORD* pUnk16;//bunch of floats, nothing happens when changing them maybe center/position?
	DWORD name;//crc32
	DWORD* pUnk17;//similar to pUnk16

	//004fea30 00412230
	EXTERN static SuperSector* GetSuperSector(DWORD checksum)
	{
		//code to get index from checksum
		static DWORD* pSectors = 0;
		_asm mov eax, [0x0085A4B8];
		_asm mov eax, [eax];
		_asm test eax, eax;
		_asm jne con;
		_asm xor eax, eax;
		_asm ret;
	con:
		_asm mov edx, checksum;
		_asm and edx, 0x0000FFFF;
		_asm lea edx, [edx + edx * 2];
		_asm lea ecx, [eax + edx * 4];
		_asm mov pSectors, ecx;


		//the SuperSectors are stored in a list 
		//each item is 8 bytes, the first 4 bytes is checksum and last 4 bytes is pointer to the SuperSector
		//since 2 or more checksums can have the same index we need to loop until we get a checksum match
		//if we find an uninitialized item before we get a checksum match it means the checksum is not in the list
		while (*pSectors != 0)//Continue until found an uninitialized item
		{

			if (*pSectors == checksum)//Checksum match
			{
				pSectors++;//skip 4 bytes to get the pointer
				return (SuperSector*)*pSectors;//return the pointer
			}
			pSectors += 2;//skip 8 bytes to get next item in the list
		}
		MessageBox(0, "return NULL", "", 0);//Checksum is not in the list
		return NULL;
	}

	//Used in the scripts create/kill/shatter/visible/invisible, the state will get updated the next frame
	void SetState(MeshState state)
	{

		typedef void(__cdecl* const pSetMeshState)(DWORD index, DWORD state, CScript* pScript);
		CScript pScript;
		pSetMeshState(0x00418DD0)(Node::GetNodeIndex(name), state, &pScript);
	}
};
//bool second = true;

struct MovingObject
{
	enum
	{
		// not used
		// MOVINGOBJ_STATUS_IDLE 				= 0,

		MOVINGOBJ_STATUS_ROTX = (1 << 0),
		MOVINGOBJ_STATUS_ROTY = (1 << 1),
		MOVINGOBJ_STATUS_ROTZ = (1 << 2),
		MOVINGOBJ_STATUS_MOVETO = (1 << 3),
		MOVINGOBJ_STATUS_ON_PATH = (1 << 4),
		MOVINGOBJ_STATUS_QUAT_ROT = (1 << 5),
		MOVINGOBJ_STATUS_LOOKAT = (1 << 6), // will be on in addition to rotx, y or z flags.
		MOVINGOBJ_STATUS_HOVERING = (1 << 7),
		MOVINGOBJ_STATUS_JUMPING = (1 << 8),
		MOVINGOBJ_STATUS_FOLLOWING_LEADER = (1 << 9),

		// moved into lock obj component
		// MOVINGOBJ_STATUS_LOCKED_TO_OBJECT	= ( 1 << 10 ),

		// don't do any other processing, until this flag is off:
		// wasn't being used...
		// MOVINGOBJ_STATUS_HIGH_LEVEL 		= ( 1 << 31 ), // running C-code on a high level.
	};//From thug1source, currently not used

	MeshState state;
	DWORD rotation;
	enum Types
	{
		MOVE_TO_POS,
		MOVE_TO_NODE,
		FOLLOW_PATH_LINKED,
		ANGULAR_VELOCITY
	};
	Types Type;
	float timer;
	float end;
	float speed;
	D3DXVECTOR3 goal;
	D3DXVECTOR3 pos;
	union
	{
		D3DXVECTOR3 angle;
		D3DXVECTOR3 acc;
	};
	D3DXVECTOR3 goalAngle;
	SuperSector* sector;
	CStructHeader* link;
	CScript* pScript;
	D3DXVECTOR3 bboxMax;
	D3DXVECTOR3 bboxMin;
	D3DXVECTOR3* vertices;
	D3DXVECTOR3 acceleration;
	D3DXVECTOR3 deceleration;
	Matrix orient;
	Matrix position;
	Matrix velocity;
	


	MovingObject(SuperSector* _sector, D3DXVECTOR3& _goal, CScript* _pScript)
	{
		_printf("MOVE_TO_NODE\n");
		state = MeshState::create;
		Type = MOVE_TO_NODE;
		link = NULL;
		sector = _sector;
		this->pScript = _pScript;
		pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
		angle = Vertex(0.0f, 0.0f, 0.0f);
		goal = _goal;
		timer = 0.0f;
		float distance = D3DXVec3Length(&(pos - goal));
		speed = 100.0f;
		end = distance / speed;

		vertices = NULL;
	}

	void Kill()
	{
		state = MeshState::kill;
	}

	MovingObject(SuperSector* _sector, D3DXVECTOR3& _angle, CScript* _pScript, DWORD checksum)
	{
		_printf("ANGLULAR_VELOCITY\n");
		state = MeshState::create;
		Type = ANGULAR_VELOCITY;
		sector = _sector;
		this->link = Node::GetNodeStruct(checksum);
		pScript = _pScript;
		if (!this->link)
			MessageBox(0, "", "", 0);
		CStructHeader* _position;
		if (!this->link->GetStruct(Checksums::Position, &_position))
			pos = D3DXVECTOR3(0, 0, 0);
		else
			pos = *_position->pVec;


		/*angle = _angle;
		angle *= 0.0015339794921875f;*/
		angle = D3DXVECTOR3(0,0,0);
		acceleration = _angle * D3DX_PI/2048.0f;
		goalAngle= D3DXVECTOR3(0, 0, 0);

		bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		//vertices = new D3DXVECTOR3[sector->numVertices];
		vertices = NULL;

		for (DWORD i = 0; i < sector->numVertices; i++)
		{
			if (sector->vertices[i].x > bboxMax.x)
				bboxMax.x = sector->vertices[i].x;
			if (sector->vertices[i].x < bboxMin.x)
				bboxMin.x = sector->vertices[i].x;

			if (sector->vertices[i].y > bboxMax.y)
				bboxMax.y = sector->vertices[i].y;
			if (sector->vertices[i].y < bboxMin.y)
				bboxMin.y = sector->vertices[i].y;

			if (sector->vertices[i].z > bboxMax.z)
				bboxMax.z = sector->vertices[i].z;
			if (sector->vertices[i].z < bboxMin.z)
				bboxMin.z = sector->vertices[i].z;
			//vertices[i] = sector->vertices[i] - pos;
		}
		sector->bboxMax = bboxMax;
		sector->bboxMin = bboxMin;
		bboxMax -= pos;
		bboxMin -= pos;
		pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
		D3DXMatrixTranslation(&position, pos.x, pos.y, pos.z);
		D3DXMatrixIdentity(&orient);
		D3DXMatrixIdentity(&velocity);
	}

	/*bool SetUpLookAtPos(const Vertex& lookToPos, const Vertex& currentPos, int headingAxis, int rotAxis, float threshold)
	{
		Vertex pathHeading = lookToPos - currentPos;

		angle[rotAxis] = 0.0f;
		goalAngle[rotAxis] = GetAngle(orient, pathHeading, headingAxis, rotAxis);
		if (fabs(goalAngle[rotAxis]) > threshold)
		{
			rotation |= (MOVINGOBJ_STATUS_ROTX << rotAxis);
			return true;
		}

		return false;
	}*/


	MovingObject(SuperSector* _sector, D3DXVECTOR3 & _pos, D3DXVECTOR3& _goal, CScript* _pScript, CStructHeader* _node)
	{
		_printf("FOLLOW_PATH_LINKED\n");
		Type = FOLLOW_PATH_LINKED;
		state = MeshState::create;
		sector = _sector;

		angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		goal = _goal;
		pos = _pos;
		timer = 0.0f;
		float distance = D3DXVec3Length(&(pos - goal));
		speed = 1000.0f;
		end = distance / speed;
		this->link = _node;
		this->pScript = _pScript;
		vertices = NULL;
		bboxMax = sector->bboxMax - pos;
		bboxMin = sector->bboxMin - pos;
		//std::copy(sector->vertices, sector->vertices + sector->numVertices, vertices);
		/*for (DWORD i = 0; i < sector->numVertices; i++)
		{
			vertices[i] = sector->vertices[i];
		}*/
		/*CStructHeader* angles;
		if (!_pScript->node->GetNodeStruct()->GetStruct(Checksums::Angles, &angles))
			angle = D3DXVECTOR3(0, 0, 0);
		else
			angle = *angles->pVec;*/

		/*if (!link->GetStruct(Checksums::Angles, &angles))
			goalAngle = D3DXVECTOR3(0, 0, 0);
		else
			goalAngle = *angles->pVec;*/

		/*CStructHeader* angles;
		if (!_pScript->node->GetNodeStruct()->GetStruct(Checksums::Angles, &angles))
			angle = D3DXVECTOR3(0, 0, 0);
		else
			angle = *angles->pVec;
		D3DXMatrixIdentity(&orient);
		D3DXMatrixRotationYawPitchRoll(&orient, angle.y, angle.x, angle.z);*/
		D3DXMATRIX nodeRotation;
		D3DXMatrixIdentity(&nodeRotation);
		D3DXMatrixIdentity(&orient);
		//D3DXVECTOR3 pathHeading = goal - pos;
		//goalAngle = //Vertex(0, GetAngle(orient, *(Vertex*)&pathHeading, Z, Y), 0);// 
			goalAngle = D3DXVECTOR3(0, atan2f((pos.x - goal.x), (pos.z - goal.z)), 0);//D3DXVECTOR3(0, AngleY(orient, pos, goal), 0);
		//angle.y = D3DX_PI;
		//goalAngle += angle;
		
		/*D3DXVECTOR3 lookAt = goalAngle - angle;
					_printf("current lookAt %f %f %f\n", lookAt.x, lookAt.y, lookAt.z);
					D3DXVec3Normalize(&lookAt, &lookAt);
					lookAt *= delta;
					_printf("delta lookAt %f %f %f\n", lookAt.x, lookAt.y, lookAt.z);*/
		/*(*(Matrix*)&orient).RotateYLocal(goalAngle.y);
		(*(Matrix*)&orient).OrthoNormalizeAbout2(Y);*/

		/*direction = goal - pos;
		D3DXVec3Normalize(&Velocity, &direction);
		Velocity *= speed * delta;

		D3DXMatrixTranslation(&nodeTranslation, Velocity.x + pos.x, Velocity.y + pos.y, Velocity.z + pos.z);
		D3DXMatrixMultiply(&world, &nodeRotation, &nodeTranslation)*/


		bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		sector->bboxMax = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		sector->bboxMin = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (DWORD i = 0; i < sector->numVertices; i++)
		{
			sector->vertices[i] -= pos;
			sector->vertices[i] = Transform(sector->vertices[i], orient);
			sector->vertices[i] += pos;
			if (bboxMax.x < sector->vertices[i].x)
				bboxMax.x = sector->vertices[i].x;
			if (bboxMin.x > sector->vertices[i].x)
				bboxMin.x = sector->vertices[i].x;

			if (bboxMax.y < sector->vertices[i].y)
				bboxMax.y = sector->vertices[i].y;
			if (bboxMin.y > sector->vertices[i].y)
				bboxMin.y = sector->vertices[i].y;

			if (bboxMax.z < sector->vertices[i].z)
				bboxMax.z = sector->vertices[i].z;
			if (bboxMin.z > sector->vertices[i].z)
				bboxMin.z = sector->vertices[i].z;
		}

		sector->bboxMax = bboxMax;
		sector->bboxMin = bboxMin;

		/*bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		vertices = new D3DXVECTOR3[sector->numVertices];

		for (DWORD i = 0; i < sector->numVertices; i++)
		{
			vertices[i] = sector->vertices[i] - pos;
			if (bboxMax.x < sector->vertices[i].x)
				bboxMax.x = sector->vertices[i].x;
			if (bboxMin.x > sector->vertices[i].x)
				bboxMin.x = sector->vertices[i].x;

			if (bboxMax.y < sector->vertices[i].y)
				bboxMax.y = sector->vertices[i].y;
			if (bboxMin.y > sector->vertices[i].y)
				bboxMin.y = sector->vertices[i].y;

			if (bboxMax.z < sector->vertices[i].z)
				bboxMax.z = sector->vertices[i].z;
			if (bboxMin.z > sector->vertices[i].z)
				bboxMin.z = sector->vertices[i].z;
		}
		sector->bboxMax = bboxMax;
		sector->bboxMin = bboxMin;
		bboxMax -= pos;
		bboxMin -= pos;*/

		//pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
		angle = goalAngle;
		_printf("Generated new MovingObject pos %f %f %f goal %f %f %f goalAngle %f speed %f\n", pos.x, pos.y, pos.z, goal.x, goal.y, goal.z, goalAngle.y, speed);
	}


	~MovingObject()
	{
		while (updatingObjects)
			Sleep(10);
		if (vertices && !InvalidReadPtr(vertices) && !InvalidReadPtr(vertices, sector->numVertices*sizeof(D3DXVECTOR3)))
			delete[] vertices;
		vertices=NULL;
		sector=NULL;
	}


	bool Update(float delta)
	{
		bool update = false;
		static D3DXVECTOR3 direction, Velocity;
		static D3DXMATRIX nodeRotation, nodeTranslation, world;
		D3DXMatrixIdentity(&nodeRotation);
		D3DXMatrixIdentity(&nodeTranslation);
		D3DXMatrixIdentity(&world);
		//D3DXMatrixIdentity(&orient);


		//_printf("MovingObject on the move(%f %f %f) dt:%f timer:%f end:%f\n", pos.x, pos.y, pos.z, delta, timer, end);

		timer += delta;
		switch (Type)
		{
		case MOVE_TO_POS:
			if (timer >= end)
			{
				if (pos != goal)
				{
					D3DXVECTOR3 relPos = goal - pos;

					sector->bboxMax += relPos;
					sector->bboxMin += relPos;
					for (DWORD i = 0; i < sector->numVertices; i++)
					{
						sector->vertices[i] += relPos;
					}
					update = true;
				}

				RemoveMovingObject(sector);
				_printf("MovingObject final destination\n");
				return update;
			}

			direction = goal - pos;
			D3DXVec3Normalize(&Velocity, &direction);
			Velocity *= speed * delta;

			sector->bboxMax += Velocity;
			sector->bboxMin += Velocity;
			for (DWORD i = 0; i < sector->numVertices; i++)
			{
				sector->vertices[i] += Velocity;
			}
			pos += Velocity;

			return true;

		case FOLLOW_PATH_LINKED:
			if (timer >= end)//we should be at the linked location now
			{
				if (pos != goal)//if we are not at the right location make us be
				{
					D3DXVECTOR3 relPos = goal - pos;

					sector->bboxMax += relPos;
					sector->bboxMin += relPos;
					for (DWORD i = 0; i < sector->numVertices; i++)
					{
						sector->vertices[i] += relPos;
					}

					pos += relPos;

					update = true;//Now we will update vertexbuffer even if angle is not changed
				}

				//Get the array of links
				CArray* links = link->GetArray(Checksums::Links);
				if (!links)
				{
					RemoveMovingObject(sector);
					_printf("MovingObject final destination\n");
					return update;
				}

				//If 1 link use it, else pick a random one
				int numLinks = links->GetNumItems();
				if (numLinks == 1)
					link = Node::GetNodeStructByIndex((*links)[0]);
				else
					link = Node::GetNodeStructByIndex((*links)[Rnd(numLinks)]);//Pick a random path
				if (!link)
				{
					RemoveMovingObject(sector);
					_printf("Couldn't find link[%d]...\n", (*links)[0]);
					return update;
				}

				//Get the position of the link
				CStructHeader* _pos;
				if (link->GetStruct(Checksums::Position, &_pos))
				{
					//update the goal target position
					goal = *_pos->pVec;
					//reset timer to zero
					timer = 0;
					//calculate distance between current position and target position
					float distance = D3DXVec3Length(&(pos - goal));
					//calculate end time
					end = distance / speed;
					//Calculate the angle needed to look at a position, taken from thug1src but for some reason not working?
					//goalAngle = D3DXVECTOR3(0, -AngleY(orient,pos, goal), 0);
					goalAngle = D3DXVECTOR3(0, atan2f((pos.x - goal.x), (pos.z - goal.z)), 0);
					if (fabsf(goalAngle.y) > 2 * D3DX_PI)
					{
						goalAngle.y -= 2 * D3DX_PI;
					}

					/*if (goalAngle.y)
					{
						(*(Matrix*)&orient).RotateYLocal(goalAngle.y);
						//Use OrthoNormalizeAbout2 because other one was the games original normalize funciton which seems to be bugged?
						(*(Matrix*)&orient).OrthoNormalizeAbout2(Y);

						bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
						bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
						sector->bboxMax = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
						sector->bboxMin = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

						for (DWORD i = 0; i < sector->numVertices; i++)
						{
							vertices[i] -= pos;
							D3DXVec3TransformCoord(&sector->vertices[i], &vertices[i], &orient);
							sector->vertices[i] += pos;
							if (bboxMax.x < sector->vertices[i].x)
								bboxMax.x = sector->vertices[i].x;
							if (bboxMin.x > sector->vertices[i].x)
								bboxMin.x = sector->vertices[i].x;

							if (bboxMax.y < sector->vertices[i].y)
								bboxMax.y = sector->vertices[i].y;
							if (bboxMin.y > sector->vertices[i].y)
								bboxMin.y = sector->vertices[i].y;

							if (bboxMax.z < sector->vertices[i].z)
								bboxMax.z = sector->vertices[i].z;
							if (bboxMin.z > sector->vertices[i].z)
								bboxMin.z = sector->vertices[i].z;
						}

						sector->bboxMax = bboxMax;
						sector->bboxMin = bboxMin;

						angle = goalAngle;

						return true;//send state to update vertexbuffer
					}*/
					//return the stored value since we might wanna update vertexbuffer if position was changed but not the angle
					return update;
				}

				RemoveMovingObject(sector);
				_printf("Couldn't find node -> position[%d]...\n", (*links)[0]);
				return update;
			}
			else
			{
				direction = goal - pos;
				D3DXVec3Normalize(&Velocity, &direction);
				Velocity *= speed * delta;

				if (Velocity)
				{

					/*sector->bboxMax += Velocity;
					sector->bboxMin += Velocity;*/
					pos += Velocity;

					

					if (angle.y == goalAngle.y)
					{
						sector->bboxMax += Velocity;
						sector->bboxMin += Velocity;

						for (DWORD i = 0; i < sector->numVertices; i++)
						{
							sector->vertices[i] += Velocity;
						}
					}
					else
					{
						bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
						bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
						sector->bboxMax = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
						sector->bboxMin = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

						/*D3DXVECTOR3 newAngle = goalAngle * timer / end;
						newAngle.y = ClampMin(newAngle.y, angle.y);*/
						if (fabsf(angle.y - goalAngle.y) < 0.02f)
						{
							angle = goalAngle;
						}
						else
						{
							D3DXVECTOR3 newAngle = goalAngle - angle;
							/*if (fabsf(newAngle.y) > D3DX_PI)
							{
								newAngle.y = -newAngle.y;// +D3DX_PI;
								goalAngle.y = -goalAngle.y
								//goalAngle.y = -goalAngle.y+D3DX_PI;
							}*/
							/*else if (newAngle.y < -D3DX_PI)
							{
								newAngle.y = newAngle.y - D3DX_PI;
								goalAngle.y = newAngle.y - D3DX_PI;
							}*/
							D3DXVec3Normalize(&newAngle, &newAngle);
							newAngle *= 0.01f;
							angle += newAngle;
						}
						(*(Matrix*)&orient).RotateYLocal(angle.y);
						//Use OrthoNormalizeAbout2 because other one was the games original normalize funciton which seems to be bugged?
						(*(Matrix*)&orient).OrthoNormalizeAbout2(Y);

						for (DWORD i = 0; i < sector->numVertices; i++)
						{
							//sector->vertices[i] += Velocity;
							//vertices[i] -= o;
							D3DXVec3TransformCoord(&sector->vertices[i], &vertices[i], &orient);
							sector->vertices[i] += pos;
							if (bboxMax.x < sector->vertices[i].x)
								bboxMax.x = sector->vertices[i].x;
							if (bboxMin.x > sector->vertices[i].x)
								bboxMin.x = sector->vertices[i].x;

							if (bboxMax.y < sector->vertices[i].y)
								bboxMax.y = sector->vertices[i].y;
							if (bboxMin.y > sector->vertices[i].y)
								bboxMin.y = sector->vertices[i].y;

							if (bboxMax.z < sector->vertices[i].z)
								bboxMax.z = sector->vertices[i].z;
							if (bboxMin.z > sector->vertices[i].z)
								bboxMin.z = sector->vertices[i].z;
						}
						sector->bboxMax = bboxMax;
						sector->bboxMin = bboxMin;

						//angle = newAngle;
					}

					

					return true;//send state to update vertexbuffer
				}
			}
			break;
		case ANGULAR_VELOCITY:
			if (acceleration.x || acceleration.y || acceleration.z || goalAngle.x || goalAngle.y || goalAngle.z)
			{
				goalAngle.x += acceleration.x*0.33f;//multiply acceleration by 0.33, or else we will get too big angle

				float velocityAngle = max(goalAngle.x, 0.001f);//clamp the angle to be minimum 0.001
				angle.x += velocityAngle;//add the velocity to current angle
				//the final angle now is 89.657394, but why is the object moving??
				printf("currentAngle %f\n", angle.x * 180 / D3DX_PI);// , velocityAngle);

				D3DXMatrixRotationYawPitchRoll(&nodeRotation, 0, angle.x, 0);
	

				//make a new bbox in a temp location so we can keep the actual bbox
				bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
				bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

				//temporarly make the actual bbox very big to prevent culling
				sector->bboxMax = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
				sector->bboxMin = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

				for (DWORD i = 0; i < sector->numVertices; i++)
				{
					//Use the vertices that's translated to origin
					//sector->vertices[i] -= pos;
					D3DXVec3TransformCoord(&sector->vertices[i], &vertices[i], &nodeRotation);
					sector->vertices[i] += pos;

					//calculate the new bbox into the temp location
					if (bboxMax.x < sector->vertices[i].x)
						bboxMax.x = sector->vertices[i].x;
					if (bboxMin.x > sector->vertices[i].x)
						bboxMin.x = sector->vertices[i].x;

					if (bboxMax.y < sector->vertices[i].y)
						bboxMax.y = sector->vertices[i].y;
					if (bboxMin.y > sector->vertices[i].y)
						bboxMin.y = sector->vertices[i].y;

					if (bboxMax.z < sector->vertices[i].z)
						bboxMax.z = sector->vertices[i].z;
					if (bboxMin.z > sector->vertices[i].z)
						bboxMin.z = sector->vertices[i].z;
				}

				//set the actual bbox to be same as temp bbox
				sector->bboxMax = bboxMax;
				sector->bboxMin = bboxMin;
				return true;//Returns true to update vertexbuffer
			}
			break;
		}
		return false;
	}

	
};

EXTERN extern std::vector<MovingObject> movingObjects;//List of Objects on the move
#endif