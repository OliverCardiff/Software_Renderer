#pragma once
#include "Matrix.h"
#include "Rasterizer.h"

namespace TakeOne
{
	class WorldObj
	{
	public:
		WorldObj(void);
		virtual ~WorldObj(void);

	public:
		Vector * _relPos;
		float _relYRot;
		float _relZRot;
		float _relXRot;
		Matrix _translation;
		Matrix _rotation;
		Matrix _mainRotation;
		Matrix _scaling;
		Matrix _transform;
		Matrix * _final;

	public:
		static float GetRand();
		static float GetURand();
		virtual void Update(float time);
		virtual void Draw(Rasterizer *  _r);

	public:
		void AssembleTransforms();
		void AssembleOnlyTranslate();
	};
}