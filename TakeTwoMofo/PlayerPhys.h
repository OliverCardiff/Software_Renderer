#pragma once
#include "PhysObj.h"

namespace TakeOne
{
	class PlayerPhys :
		public PhysObj
	{
	public:
		PlayerPhys(void);
		~PlayerPhys(void);

	public:
		 void Feed(Vector& vChange, bool flag1, bool flag2) ;
		 PhysicalState GetState() ;
		 Matrix& GetTranslation() ;
		 Vector& GetLocation() ;
		 void Update(float time) ;
	};
}
