#include "StdAfx.h"
#include "Camera.h"

using namespace TakeOne;

Camera::Camera(void)
{
	_depth = 1;
	_roll = 0;
	BuildProjection();
	BuildRotation();
	BuildTranslation();
	BuildFinal();
}

Camera::Camera(Vector& target, Vector& position, float roll, float fov, AspectRatio ar, Vector2& zClip, Vector2& viewPort)
{
	_lookAt = target;
	_position = position;
	_roll = roll;

	_zClipping = zClip;
	_viewPort = viewPort;
	_fov = fov;

	if(ar == SixteenNine)
	{
		_asRatio = 16.0f/9.0f;
	}
	else
	{
		_asRatio = 4.0f/3.0f;
	}
	SetDepth();
	BuildScreenTransform();
	BuildProjection();
	BuildRotation();
	BuildTranslation();
	BuildFinal();

	BuildAll();
}

Camera::~Camera(void)
{
}

//Matrix Construction

void Camera::SetDepth()
{
	_viewPlane._x = 1;
	_viewPlane._y = 1/_asRatio;

	_depth = tan(_fov/2.0f);
}
void Camera::BuildScreenTransform()
{
	float a = (_viewPort._x - 1)/2;
	float b = (_viewPort._y - 1)/2;

	_toScreen._m[0][0] = a;
	_toScreen._m[3][0] = a;
	_toScreen._m[1][1] = -b;
	_toScreen._m[3][1] = b;

}
void Camera::BuildAll()
{
	_all =_toScreen * _projection * _cFinal;
//	_all =  _projection * _cFinal;
}
void Camera::BuildProjection()
{
	_projection._m[0][0] = _depth;
	_projection._m[1][1] = _depth * _asRatio;
	_projection._m[2][2] = _depth;
	_projection._m[3][3] = 0;
	_projection._m[2][3] = 1;
}
void Camera::BuildRotation()
{
	Vector up(sin(_roll), -cos(_roll), 0);

	Vector forward(_lookAt._x - _position._x, _lookAt._y - _position._y, _lookAt._z - _position._z);
	forward.MakeUnit();

	Vector right = up.CrossProduct(forward);
	right.MakeUnit();

	up = right.CrossProduct(forward);
	up.MakeUnit();

	_cRot._m[0][0] = right._x; _cRot._m[1][0] = right._y; _cRot._m[2][0] = right._z;
	_cRot._m[0][1] = up._x; _cRot._m[1][1] = up._y; _cRot._m[2][1] = up._z;
	_cRot._m[0][2] = forward._x; _cRot._m[1][2] = forward._y; _cRot._m[2][2] = forward._z;
}

void Camera::BuildTranslation()
{
	Matrix::MakeTranslation(-_position._x, -_position._y, -_position._z, _cTrans, false);
}

void Camera::BuildFinal()
{
	_cFinal = _cRot * _cTrans;
}

//Updates
//updates only position without wasting resources updating rotation
void Camera::UpdateTranslation(Vector & change)
{
	_position.Add(change);
	BuildTranslation();
	BuildFinal();
}
//allows user to change rotation and update all
void Camera::UpdateRotation(Vector & lookChange, float rollChange)
{
	_lookAt = lookChange;
	_roll += rollChange;
	BuildRotation();
	BuildFinal();

}
//updates all without directly changing anything, just in case
void Camera::UpdateAll()
{
	BuildTranslation();
	BuildRotation();
	BuildFinal();
}
//give the opportunity to modify all properties whilst updating all
void Camera::UpdateAll(Vector & lookChange, Vector & posChange, float rollChange)
{
	_lookAt = lookChange;
	_position = posChange;
	_roll = rollChange;

	BuildTranslation();
	BuildRotation();
	BuildFinal();

	BuildAll();
}

//applies a transformation matrix to a the camera's position
void Camera::TransformPosition(Matrix & change)
{
	Matrix::Transform(change, _position);

}
//applies a transformation matrix to a the camera's target
void Camera::TransformTarget(Matrix & change)
{
	Matrix::Transform(change, _lookAt);

}

//Updates the depth for the new field of view
void Camera::SetFov(float fov)
{
	_fov = fov;
	SetDepth();
	BuildProjection();
}

//Get functions
//returns the zdependant clip planes
Vector2 Camera::GetClipZ()
{
	return _zClipping;
}


Vector2 Camera::GetViewPort()
{
	return _viewPort;
}

Matrix& Camera::GetView()
{
	return _cFinal;
}

Matrix& Camera::GetProjection()
{
	return _projection;
}

Matrix& Camera::GetToScreen()
{
	return _toScreen;
}

Matrix Camera::GetToScreenProjection()
{
	return  _toScreen * _projection;
}

Matrix& Camera::GetAll()
{
	return _all;
}
float Camera::GetDepth()
{
	return _depth;
}