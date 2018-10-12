#pragma once
#include "Rasterizer.h"
#include "Entity.h"
#include "Player.h"
#include "PhysicsManager.h"
#include "ControlManager.h"
#include "Camera.h"
#include "DemoModel.h"
#include "Terrain.h"


namespace TakeOne
{
		
	enum DrawMode 
	{
		Terrains, Models
	};
	class World
	{
	public:
		World(PhysicsManager * p, ControlManager * c, Camera * cam, Camera * cam2);
		~World(void);
	public:
		void MakeWorld();
		void Update(float time,DrawMode);
		void Draw(Rasterizer * raster, DrawMode);
	private:
		std::list<Entity*> _entities;
		Terrain * _ter_PTR;
		PhysicsManager * _physics;
		ControlManager * _ctrl;
		Camera * _cam_PTR;
		Camera * _cam_PTR2;
		Camera * _cam_FIXD;
		DemoModel * _demoModel;
	};

}