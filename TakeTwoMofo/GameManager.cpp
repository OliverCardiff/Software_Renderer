#include "StdAfx.h"
#include "GameManager.h"
#include "Bone.h"
#include "MHuman.h"
#include "Terrain.h"
#include <time.h>

#define MS_PER_DRAW 16

using namespace TakeOne;

#define PI 3.14159265f

float a = 0;
const float ScreenX = 800;
const float ScreenY = 600;
float timer = 0;

const float TRIGGER_1 = 5;
const float TRIGGER_2 = 10;
const float TRIGGER_3 = 15;
const float TRIGGER_4 = 20;
const float TRIGGER_5 = 25;
const float TRIGGER_6 = 35;
const float TRIGGER_7 = 45;
const float TRIGGER_8 = 55;
const float TRIGGER_9 = 65;
const float TRIGGER_10 = 75;
const float TRIGGER_11 = 85;
const float TRIGGER_12 = 95;

bool mutated = false;
std::wstring s2ws(const std::string& s);

GameManager::GameManager(void)
{
	
	srand( (unsigned)time( NULL ) );
	_physics = new PhysicsManager();
	_time.Begin();
	_drawTimer = 0;	
	_camera = new Camera(Vector(0,0,0), Vector(80,40,170), 0, PI/2, FourThree, Vector2(100,2050), Vector2(ScreenX,ScreenY));
	_camera2 = new Camera(Vector(800,351,800), Vector(400,500,200), 0, PI/2, FourThree, Vector2(40,2050), Vector2(ScreenX,ScreenY));
	_fixed = new Camera(Vector(0,0,0), Vector(80,300,170), 0, PI/2, FourThree, Vector2(100,2050), Vector2(ScreenX,ScreenY));
	_fixed2 = new Camera(Vector(400,0,400), Vector(1000,1000,500), 0, PI/2, FourThree, Vector2(100,2050), Vector2(ScreenX,ScreenY));
	_raster = new Rasterizer(_camera);
	_wndPos = new Gdiplus::PointF();

	_wrld = new World(_physics, &_control, _camera, _camera2);
	_time.Update();

}
GameManager::~GameManager(void)
{
}
void GameManager::Update(HWND& hWnd)
{
	_time.Update();

	timer += (_time.GetElapsedMS())/1000;

	_control.Update(_time.GetElapsedMS(), &_input);
	_physics->Update(_time.GetElapsedMS());
	
	if(timer < TRIGGER_5)
	{
		_wrld->Update(_time.GetElapsedMS(), Models);
	}
	else
	{
		_wrld->Update(_time.GetElapsedMS(), Terrains);
	}

	_drawTimer += _time.GetElapsedMS();


	if(_drawTimer > MS_PER_DRAW)
	{
		InvalidateRect(hWnd, NULL, false);
		_drawTimer = 0;
	}

}
void GameManager::Draw(HDC& hdc)
{
	Gdiplus::Graphics g(hdc);


	if(timer < TRIGGER_1)
	{
		_wrld->Draw(_raster, Models);
		_raster->DrawWireFrame(hdc, *_fixed);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)0,(Gdiplus::REAL)0, (Gdiplus::REAL)800, (Gdiplus::REAL)600);

	}
	else if (timer < TRIGGER_2)
	{
		_wrld->Draw(_raster, Models);
		_raster->DrawFlat(hdc, *_fixed);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)0,(Gdiplus::REAL)0, (Gdiplus::REAL)800, (Gdiplus::REAL)600);
	}
	else if (timer < TRIGGER_3)
	{
		_wrld->Draw(_raster, Models);
		_raster->Draw(hdc, *_fixed);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)0,(Gdiplus::REAL)0, (Gdiplus::REAL)800, (Gdiplus::REAL)600);
	}
	else if (timer < TRIGGER_4)
	{
		_wrld->Draw(_raster, Models);
		_raster->DrawFence(hdc, *_fixed);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)0,(Gdiplus::REAL)0, (Gdiplus::REAL)800, (Gdiplus::REAL)600);
	}
	else if (timer < TRIGGER_5)
	{
		_wrld->Draw(_raster, Models);
		_raster->DrawPhong(hdc, *_fixed);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)0,(Gdiplus::REAL)0, (Gdiplus::REAL)800, (Gdiplus::REAL)600);
	}
	else if (timer < TRIGGER_6)
	{
		_wrld->Draw(_raster, Terrains);
		_raster->DrawWireFrame(hdc, *_camera);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)0,(Gdiplus::REAL)0, (Gdiplus::REAL)400, (Gdiplus::REAL)600);
		_raster->DrawWireFrame(hdc, *_camera2);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)400,(Gdiplus::REAL)0, (Gdiplus::REAL)400, (Gdiplus::REAL)600);
		
		if(!mutated)
		{
			mutated = true;
			_raster->MutateLights();
		}
	}
	else if (timer < TRIGGER_7)
	{
		_wrld->Draw(_raster, Terrains);
		_raster->DrawFlat(hdc, *_camera);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)0,(Gdiplus::REAL)0, (Gdiplus::REAL)400, (Gdiplus::REAL)600);
		_raster->DrawFlat(hdc, *_camera2);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)400,(Gdiplus::REAL)0, (Gdiplus::REAL)400, (Gdiplus::REAL)600);		
	}
	else if (timer < TRIGGER_8)
	{
		_wrld->Draw(_raster, Terrains);
		_raster->Draw(hdc, *_camera);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)0,(Gdiplus::REAL)0, (Gdiplus::REAL)800, (Gdiplus::REAL)600);	
	}
	else if (timer < TRIGGER_9)
	{
		_wrld->Draw(_raster, Terrains);
		_raster->DrawFence(hdc, *_fixed2);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)0,(Gdiplus::REAL)0, (Gdiplus::REAL)800, (Gdiplus::REAL)600);	
	}
	else if (timer < TRIGGER_10)
	{
		_wrld->Draw(_raster, Terrains);
		_raster->DrawPhong(hdc, *_fixed2);
		g.DrawImage(_raster->_bitmap, (Gdiplus::REAL)0,(Gdiplus::REAL)0, (Gdiplus::REAL)800, (Gdiplus::REAL)600);	
	}
	else
	{
		timer = 0;
		mutated = false;
		_raster->MakePointLights();
	}
	
	float nextfps = 1000/_time.GetElapsedMS();
	if(nextfps != 1000)
		_fps = nextfps;

	TCHAR arr[20];
	TCHAR arr2[40];
	std::wstring  wstr = s2ws(_raster->_CurrentRender);
	std::wstring  wstr2 = s2ws("Current Polys: %d");
	std::wstring  wstr3 = s2ws("Current Verts: %d");
	std::wstring  wstr4 = s2ws("FrameRate: %d");

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor( hdc, RGB(255, 255, 255));

	wsprintf(arr, wstr.c_str());
	TextOut(hdc, 10, 10, arr, lstrlen(arr));

	wsprintf(arr, wstr2.c_str(), _raster->_onScrPolys);
	TextOut(hdc, 10, 30, arr, lstrlen(arr));

	wsprintf(arr, wstr3.c_str(), _raster->_onScrVerts);
	TextOut(hdc, 10, 50, arr, lstrlen(arr));

	wsprintf(arr, wstr4.c_str(), (int)_fps);
	TextOut(hdc, 10, 70, arr, lstrlen(arr));

	_raster->ClearInfo();
}

void GameManager::GetInput(int key, bool down)
{
	if(key > 255) key = 255;

	if(down)
		_input.SetDown(key);
	else
		_input.SetUp(key);
}

void GameManager::GetMouseInput(POINT& p)
{
	_input.SetMouse(p);
	_input.SetWndPos(_wndPos);
}

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}