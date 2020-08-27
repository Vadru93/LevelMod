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