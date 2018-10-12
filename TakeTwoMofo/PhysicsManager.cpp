#include "StdAfx.h"
#include "PhysicsManager.h"
#include "Terrain.h"

using namespace TakeOne;

PhysicsManager::PhysicsManager(void)
{
}

PhysicsManager::~PhysicsManager(void)
{
}

void PhysicsManager::TerrainCollide(TakeOne::Vector &pos, TakeOne::Vector &vel)
{
	float farBorder = (FIELD_X - 1) * BOX_SIZE;
	InsideBox(farBorder, 0, farBorder, 0, pos, vel);

	Vector tp = pos + vel;
	
	float scPosX = tp._x / BOX_SIZE;
	float scPosZ = tp._z / BOX_SIZE;

	float height = GetHeightAt(scPosX, scPosZ);

	if(tp._y < height)
	{
		pos._y = height;
		vel._y = 0;
	}
	
}
Terrain * PhysicsManager::GetTerrain()
{
	return &_terrain;
}
float PhysicsManager::GetHeightAt(float scPosX, float scPosZ)
{
	int xLower = (int)scPosX;
	int xHigher = xLower + 1;
	float xRel = (scPosX - xLower)/((float)xHigher - (float)xLower);

	int zLower = (int)scPosZ;
	int zHigher = zLower + 1;
	float zRel = (scPosZ - zLower)/((float)zHigher - (float)zLower);
	
	bool aboveLower = (xRel + zRel< 1);

	float finalY;

	if(aboveLower)
	{
		finalY = _terrain._field[xLower][zLower]._y;
		finalY += zRel * (_terrain._field[xLower][zHigher]._y - _terrain._field[xLower][zLower]._y);
		finalY += xRel * (_terrain._field[xHigher][zHigher]._y - _terrain._field[xLower][zLower]._y);
	}
	else
	{
		finalY = _terrain._field[xHigher][zHigher]._y;
		finalY += (1.0f - zRel) * (_terrain._field[xHigher][zLower]._y - _terrain._field[xHigher][zHigher]._y);
		finalY += (1.0f - xRel) * (_terrain._field[xLower][zHigher]._y - _terrain._field[xHigher][zHigher]._y);
	}

	return finalY;
}
void PhysicsManager::InsideBox(float farX, float nearX, float farZ, float nearZ, Vector& pos, Vector& vel)
{
	if(pos._x + vel._x < nearX)
	{
		pos._x = nearX;
		vel._x = 0;
	}
	else if(pos._x + vel._x > farX)
	{
		pos._x = farX;
		vel._x = 0;
	}

	if(pos._z + vel._z < nearZ)
	{
		pos._z = nearZ;
		vel._z = 0;
	}
	else if(pos._z + vel._z > farZ)
	{
		pos._z = farZ;
		vel._z = 0;
	}
}

void PhysicsManager::Update(float time)
{
	std::list<PhysObj*>::iterator i = _movers.begin();

	for(;i != _movers.end(); i++)
	{
		TerrainCollide(*(*i)->_relPos, (*i)->_velocity);
		(*i)->Update(time);
	}

	std::list<PhysObj*>::iterator j = _static.begin();

	for(;j != _static.end(); j++)
	{
		(*j)->Update(time);
	}
}
void PhysicsManager::Submit(PhysObj * p, bool mover)
{
	if(mover)
	{
		_movers.push_back(p);
	}
	else
	{
		_static.push_back(p);
	}
}

float PhysicsManager::Distance(int x, int y, float x1, float y1)
{
	float _dx = x - x1;
	float _dy = y - y1;

	return sqrt(_dx * _dx + _dy * _dy);
}