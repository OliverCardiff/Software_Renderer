#pragma once
#include "Entity.h"
#include "PhysicsManager.h"
#include "ControlManager.h"
#include "Camera.h"

namespace TakeOne
{

	class Player :
		public Entity
	{
	public:
		Player(PhysicsManager * pm, ControlManager * cm, Vector position, Camera * cam, int ID);
		~Player(void);

	public:
		void SetPosition(Vector p);
		int Identity;
	};

}