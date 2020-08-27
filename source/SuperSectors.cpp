#define NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "SuperSectors.h"
#include "Model.h"

bool updatingObjects = false;

EXTERN std::vector<MovingObject> movingObjects;//List of Objects on the move

MovingObject::MovingObject(SuperSector* _sector, Model* mdl)
{
	_printf("FOLLOW_MODEL\n");
	model = mdl;
	state = MeshState::create;
	Type = FOLLOW_MODEL;
	link = NULL;
	sector = _sector;
	this->pScript = NULL;
	pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
	angle = Vertex(0.0f, 0.0f, 0.0f);
	goal = Vertex(0, 0, 0);
	timer = 0.0f;
	float distance = 0.0f;
	speed = 0.0f;
	end = 0.0f;
	vertices = NULL;
}

bool MovingObject::Update(float delta)
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
	case FOLLOW_MODEL:
		if (model->pos != pos)
		{
			D3DXMATRIX translation;
			D3DXMATRIX result;
			D3DXMatrixTranslation(&translation, model->pos.x, model->pos.y, model->pos.z);
			D3DXMatrixMultiply(&result, &model->rotation, &translation);

			//Move the bbox to origin
			sector->bboxMax = model->pos + (sector->bboxMax - pos);
			sector->bboxMin = model->pos - (sector->bboxMin + pos);

			//Rotate and move the bbox according to model
			D3DXVec3TransformCoord(&sector->bboxMax, &sector->bboxMax, &result);
			D3DXVec3TransformCoord(&sector->bboxMin, &sector->bboxMin, &result);

			//Check if the rotation made bbox flip axis
			if (sector->bboxMin.x > sector->bboxMax.x)
			{
				const float temp = sector->bboxMax.x;
				sector->bboxMax.x = sector->bboxMin.x;
				sector->bboxMin.x = temp;
			}
			if (sector->bboxMin.y > sector->bboxMax.y)
			{
				const float temp = sector->bboxMax.y;
				sector->bboxMax.y = sector->bboxMin.y;
				sector->bboxMin.y = temp;
			}
			if (sector->bboxMin.z > sector->bboxMax.z)
			{
				const float temp = sector->bboxMax.z;
				sector->bboxMax.z = sector->bboxMin.z;
				sector->bboxMin.z = temp;
			}

			//Rotate and move the vertices according to model using the origin vertices
			for (DWORD i = 0; i < sector->numVertices; i++)
			{
				D3DXVec3TransformCoord(&sector->vertices[i], &vertices[i], &result);
			}

			//Update local position
			pos = model->pos;
		}
		break;
	case ANGULAR_VELOCITY:
		if (acceleration.x || acceleration.y || acceleration.z || goalAngle.x || goalAngle.y || goalAngle.z)
		{
			goalAngle.x += acceleration.x * 0.33f;//multiply acceleration by 0.33, or else we will get too big angle

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