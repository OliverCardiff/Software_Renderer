#pragma once
#include "Rasterizer.h"
#include "Model.h"
#include "Controller.h"
#include "PhysObj.h"

namespace TakeOne
{
	class Entity
	{
	public:
		Entity(Vector position);
		virtual ~Entity(void);
	public:
		void Update(float time);
		void Draw(Rasterizer * _r);
	protected:
		Model * _model;
		Controller * _control;
		PhysObj * _physObj;
		bool _flag1;
		bool _flag2;

	};
}
