#include "StdAfx.h"
#include "World.h"

using namespace TakeOne;

const Vector PLAYER_START(200,0,200);
const Vector PLAYER2_START(400,0,400);

World::World(PhysicsManager * p, ControlManager * c, Camera * cam, Camera * cam2)
{
	_ctrl = c;
	_physics = p;
	_ter_PTR = p->GetTerrain();
	_cam_PTR = cam;
	_cam_PTR2 = cam2;

	MakeWorld();
}

World::~World(void)
{
}
void World::MakeWorld()
{
	Player * p  = new Player(_physics, _ctrl,PLAYER_START, _cam_PTR, 1);
	Player * q  = new Player(_physics, _ctrl,PLAYER2_START, _cam_PTR2, 2);
	_demoModel = new DemoModel();

	_entities.push_back(p);
	_entities.push_back(q);
	

}
void World::Update(float time, DrawMode mode)
{
	if(mode == Terrains)
	{
	std::list<Entity*>::iterator i = _entities.begin();

	for(;i != _entities.end(); i++)
	{
		(*i)->Update(time);
	}
	}
	else if(mode == Models)
	{
		_demoModel->Animate(time, 1);
	}

}
void World::Draw(Rasterizer * raster, DrawMode mode)
{
	if(mode == Terrains)
	{
		std::list<Entity*>::iterator i = _entities.begin();

		for(;i != _entities.end(); i++)
		{
			(*i)->Draw(raster);
		}

		_ter_PTR->Draw(raster);
	}
	else if(mode == Models)
	{
		_demoModel->Draw(raster);
	}
}

