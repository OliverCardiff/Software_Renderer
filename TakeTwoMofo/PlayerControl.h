#pragma once
#include "Controller.h"
#include "Camera.h"

namespace TakeOne
{
	class PlayerControl :
		public Controller
	{
	public:
		PlayerControl(Camera *, int);
		~PlayerControl(void);

		 void Update(float time, InputHandler * IH);
		 Matrix GetTranslation();
		 Matrix GetScaling();
		 Matrix GetRotation();
		 Vector GetVChange();
		 void Feed(Vector& Location, PhysicalState p);
		 int GetAnimationType();

	public:
		Camera * _cam;

	private:
		Matrix _mRotY;
		Matrix _mRotX;
		void ToVelocityXZ(float angle, Vector& vec);
		float _YRot;
		PhysicalState _state;
		float _XRot;
		Vector _nextVelocity;
		Vector _location;
		int Identity;
		Vector _camPos;
	};

}