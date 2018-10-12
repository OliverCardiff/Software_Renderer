#pragma once
#include "Model.h"
#include "Bone.h"

namespace TakeOne
{

	class MHuman :
		public Model
	{
	public:
		MHuman(void);
		virtual ~MHuman(void);
		void Animate(float time, int type);
	private:
		void BuildHuman();
		void KeepStill();
		void BuildFragment(Bone*& b, float poleHeight, float boxHeight, float sqX, float sqZ);
		void LimbMove(float maxAngle, float wholeOffset, float secondOffset, float angleMod, float maxBoost, float speed, Bone * a, Bone * b, float time);
		void TranslateXZ(Vector& first, Vector& second, float angle, float length);
	private:
		Bone * _leg1a;
		Bone * _leg1b;
		Bone * _leg2a;
		Bone * _leg2b;
		Bone * _torso;
		Bone * _arm1a;
		Bone * _arm1b;
		Bone * _arm2a;
		Bone * _arm2b;
		Bone * _head;

	private:
		Matrix _rotOne;
		Matrix _transOne;
		float _timer;

	};

}
