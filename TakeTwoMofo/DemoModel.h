#pragma once
#include "Model.h"

namespace TakeOne
{
	class DemoModel :
		public Model
	{
	public:
		DemoModel(void);
		~DemoModel(void);
		void Animate(float time, int type);
		Bone * _demo;

	private:
		float _rota;
		float _rotb;
		float _rotc;

	};
}
