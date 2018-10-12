#pragma once
#include "InputHandler.h"
#include "Matrix.h"
#include "Vector.h"

namespace TakeOne
{
	class Controller
	{
	public:
		Controller(void);
		virtual ~Controller(void);

	public:
		virtual void Update(float time, InputHandler * IH) = 0;
		virtual Matrix GetTranslation() = 0;
		virtual Matrix GetScaling() = 0;
		virtual Matrix GetRotation() = 0;
		virtual Vector GetVChange() = 0;
		virtual void Feed(Vector& Location, PhysicalState p) = 0;
		virtual int GetAnimationType() = 0;
	};

}