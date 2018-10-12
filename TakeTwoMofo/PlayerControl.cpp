#include "StdAfx.h"
#include "PlayerControl.h"

using namespace TakeOne;

const Vector START_OFFSET(-200, 150, 0);
const float MAX_SPEED = 200;
const float SENSITIVITY = 0.005f;
const float JUMP_AMOUNT = 10;

PlayerControl::PlayerControl(Camera * cam, int ID)
{
	Identity = ID;
	_cam = cam;
	_camPos = START_OFFSET;
	_YRot = 0;
	_XRot = 0;
}

PlayerControl::~PlayerControl(void)
{
}

 void PlayerControl::Update(float time, InputHandler * IH)
 {
	 if(Identity == 1)
	 {
		 _camPos = START_OFFSET;
		 _nextVelocity.Multiply(0);

		 Gdiplus::PointF * p = IH->GetMouse();
		 Gdiplus::PointF * w = IH->GetWindowPos();

		 float xChange = p->X - 400;
		 float yChange = p->Y - 300;

		 _YRot += xChange * SENSITIVITY;
		 _XRot += yChange * SENSITIVITY;

		 if(_XRot < 3*PI/2) _XRot = 3*PI/2;
		 if(_XRot > 2* PI) _XRot = 2*PI;
		
		 Matrix::MakeRotation(Y, -_YRot, _mRotY, false);
		 
		
		 Matrix::Transform(_mRotY, _camPos);

		 _camPos = _camPos + _location;

		 SetCursorPos((int)w->X + 400, (int)w->Y + 300);	

		 if(IH->IsDown(KEY_W))
		 {
			 ToVelocityXZ(-_YRot + PI/2, _nextVelocity);
		 }
		 if(IH->IsDown(KEY_S))
		 {
			 ToVelocityXZ(-_YRot - PI/2, _nextVelocity);
		 }
		 if(IH->IsDown(KEY_A))
		 {
			 ToVelocityXZ(-_YRot + PI , _nextVelocity);
		 }
		 if(IH->IsDown(KEY_D))
		 {
			 ToVelocityXZ(-_YRot, _nextVelocity);
		 }
		 
		 _nextVelocity.MakeUnit();
		 _nextVelocity.Multiply(MAX_SPEED * (time/1000));

		if(IH->IsDown(KEY_SPACE))
		 {
			 _nextVelocity._y = JUMP_AMOUNT;
		 }
	 }
	 else if(Identity == 2)
	 {
		 _camPos = START_OFFSET;
		 _nextVelocity.Multiply(0);

		 _camPos = _camPos + _location;

		 if(IH->IsDown(KEY_I))
		 {
			 ToVelocityXZ(-_YRot + PI/2, _nextVelocity);
		 }
		 if(IH->IsDown(KEY_K))
		 {
			 ToVelocityXZ(-_YRot - PI/2, _nextVelocity);
		 }
		 if(IH->IsDown(KEY_J))
		 {
			 ToVelocityXZ(-_YRot + PI , _nextVelocity);
		 }
		 if(IH->IsDown(KEY_L))
		 {
			 ToVelocityXZ(-_YRot, _nextVelocity);
		 }
		 
		 _nextVelocity.MakeUnit();
		 _nextVelocity.Multiply(MAX_SPEED * (time/1000));

		if(IH->IsDown(KEY_B))
		 {
			 _nextVelocity._y = JUMP_AMOUNT;
		 }
	 }

	 _cam->UpdateAll(_location, _camPos, 0);
 }
 void PlayerControl::ToVelocityXZ(float angle, Vector& vec)
 {
	vec._x += sin(angle);
	vec._z += cos(angle);
 }
 Matrix PlayerControl::GetTranslation()
 {
	 return Matrix();
 }
 Matrix PlayerControl::GetScaling()
 {
	 return Matrix();
 }
 Matrix PlayerControl::GetRotation()
 {
	 Matrix m;
	 Matrix::MakeRotation(Y, -_YRot, m, false);
	 return m;
 }
 Vector PlayerControl::GetVChange()
 {
	 return _nextVelocity;
 }
 void PlayerControl::Feed(Vector& location, PhysicalState p)
 {
	_location = location;
	_location._y += 50;
	_state = p;
 }
 int PlayerControl::GetAnimationType()
 {
	 return int(_state);
 }