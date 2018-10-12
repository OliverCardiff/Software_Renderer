#pragma once
#include "Controller.h"
#include "Camera.h"

namespace TakeOne
{

	class AIController :
		public Controller
	{
	public:
		AIController(Camera *, int);
		~AIController(void);
	public:
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
		void Bounce();

		float _xChange;
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