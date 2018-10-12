#pragma once
#include "Gametime.h"
#include "InputHandler.h"
#include "Rasterizer.h"
#include "PhysicsManager.h"
#include "ControlManager.h"
#include "Camera.h"
#include "Terrain.h"
#include "World.h"

namespace TakeOne
{
	class GameManager
	{
	public:
		GameManager(void);
		~GameManager(void);
	public:
		void Update(HWND& hWnd); //main update func for game
		void Draw(HDC& hdc); //main draw func for game
		void GetInput(int, bool);
		void GetMouseInput(POINT& p);
		Gdiplus::PointF * _wndPos;

	private:
		float _drawTimer;
		InputHandler _input;
		GameTime _time;
		Camera * _camera;
		Camera * _camera2;
		Camera * _fixed;
		Camera * _fixed2;
		Rasterizer * _raster;
		PhysicsManager * _physics;
		ControlManager _control;
		World * _wrld;
		float _fps;
	};
}