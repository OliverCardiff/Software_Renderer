#include "StdAfx.h"
#include "InputHandler.h"

using namespace TakeOne;

InputHandler::InputHandler(void)
{
	AllUp();
}

InputHandler::~InputHandler(void)
{
}

void InputHandler::SetDown(int k)
{
	_keys[k] = true;
}
void InputHandler::SetUp(int k)
{
	_keys[k] = false;
}

void InputHandler::AllUp()
{
	for(int i = 0;i < 256; i++)
	{
		_keys[i] = false;
	}
}

bool InputHandler::IsDown(int key)
{
	return _keys[key];
}

void InputHandler::SetMouse(POINT &p)
{
	_mouse.X = (Gdiplus::REAL)p.x;
	_mouse.Y = (Gdiplus::REAL)p.y;

}
void InputHandler::SetWndPos( Gdiplus::PointF * wp)
{
	_window = wp;
}
Gdiplus::PointF * InputHandler::GetWindowPos()
{
	return _window;
}
Gdiplus::PointF * InputHandler::GetMouse()
{
	return &_mouse;
}