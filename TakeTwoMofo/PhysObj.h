#pragma once
#include "WorldObj.h"

namespace TakeOne
{
	class PhysObj :
		public WorldObj
	{
	public:
		PhysObj(void);
		virtual ~PhysObj(void);

	public:
		virtual void Feed(Vector& vChange, bool flag1, bool flag2) = 0;
		virtual PhysicalState GetState() = 0;
		virtual Matrix& GetTranslation() = 0;
		virtual Vector& GetLocation() = 0;
		virtual void Update(float time) = 0;
		Vector _velocity;
	protected:
		Vector _inputV;
		PhysicalState _phys; 
	};
}