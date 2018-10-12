#pragma once
#include "WorldObj.h"

namespace TakeOne
{
	class Bone;
	class Model: public WorldObj
	{
	public:
		Model(void);
		virtual ~Model(void);

	public:
		 void Draw(Rasterizer * _r);
		 void Update(float time);
		 virtual void Animate(float time, int type);

	protected:
		std::list<Bone*> _bones;
		void BuildBoneNormals();
		Vector _wrldOffset;
		Matrix _temp;
	};
}
