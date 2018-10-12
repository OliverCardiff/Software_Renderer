#pragma once

#include "stdafx.h"

namespace TakeOne
{
	enum AspectRatio
	{
		SixteenNine, FourThree
	};
	class Camera
	{
	public://constructors
		Camera	(void);
		Camera	(Vector&, Vector&, float, float, AspectRatio, Vector2&,Vector2&);
		~Camera	(void);

	public: //Updates
		void UpdateTranslation(Vector&);
		void UpdateRotation(Vector&, float);
		void UpdateAll(Vector&, Vector&, float);
		void UpdateAll();
		void TransformPosition(Matrix&);
		void TransformTarget(Matrix&);
		void SetFov(float fov);

	public://Gets
		Vector2 GetClipZ();
		Matrix& GetView();
		Vector2 GetViewPort();
		Matrix& GetProjection();
		Matrix& GetToScreen();
		Matrix GetToScreenProjection();
		Matrix& GetAll();
		float GetDepth();
		

	private:
		void	BuildRotation();
		void	BuildTranslation();
		void	BuildFinal();
		void	BuildProjection();
		void    BuildScreenTransform();
		void	BuildAll();
		void    SetDepth();

	private:
		Vector	_lookAt;
		Vector	_position;
		float	_roll;
		//depth of viewing plane from camera
		float	_depth;
		//field of view
		float   _fov;
		//clip planes
		Vector2 _zClipping;
		//viewing plane dimensions
		Vector2 _viewPlane;
		//the aspect ratio
		float  _asRatio;
		//the viewport size
		Vector2 _viewPort;

	private: //Matrices
		Matrix	_cRot;
		Matrix	_cTrans;
		Matrix	_cFinal;
		Matrix  _projection;
		Matrix  _toScreen;
		Matrix  _toSrProj;
		Matrix  _all;
	};
}
