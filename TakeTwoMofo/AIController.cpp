#include "StdAfx.h"
#include "AIController.h"
#include "WorldObj.h"

using namespace TakeOne;


const Vector START_OFFSET(-200, 150, 0);
const float MAX_SPEED = 200;
const float SENSITIVITY = 0.005f;
const float JUMP_AMOUNT = 10;
const int BOX_SIZE = 800;

AIController::AIController(Camera * cam, int ID)
{
	Identity = ID;
	_cam = cam;
	_camPos = START_OFFSET;
	_YRot = 0;
	_XRot = 0;
	_xChange = 0;
}

AIController::~AIController(void)
{
}
 void AIController::Update(float time, InputHandler * IH)
 {
	 _camPos = START_OFFSET;
	 _nextVelocity.Multiply(0);
	
	 if(Identity == 1)
		_YRot += 0.03f;
	 else
		 _YRot -= 0.03f;

	 if(WorldObj::GetRand() > 0.9f)
	 {
		 _YRot -= WorldObj::GetRand() * 0.1;
	 }
	
	 Matrix::MakeRotation(Y, -_YRot, _mRotY, false);
	 
	 Matrix::Transform(_mRotY, _camPos);

	 _camPos = _camPos + _location;



	 ToVelocityXZ(-_YRot + PI/2, _nextVelocity);
	 
	 
	 _nextVelocity.MakeUnit();
	 _nextVelocity.Multiply( MAX_SPEED * ( time / 1000 ) );
	 
	 _cam->UpdateAll(_location, _camPos, 0);
 }
 void AIController::ToVelocityXZ(float angle, Vector& vec)
 {
	vec._x += sin(angle);
	vec._z += cos(angle);
 }
 Matrix AIController::GetTranslation()
 {
	 return Matrix();
 }
 Matrix AIController::GetScaling()
 {
	 return Matrix();
 }
 Matrix AIController::GetRotation()
 {
	 Matrix m;
	 Matrix::MakeRotation(Y, -_YRot, m, false);
	 return m;
 }
 Vector AIController::GetVChange()
 {
	 return _nextVelocity;
 }
 void AIController::Feed(Vector& location, PhysicalState p)
 {
	_location = location;
	_location._y += 50;
	_state = p;
 }
 int AIController::GetAnimationType()
 {
	 return int(_state);
 }