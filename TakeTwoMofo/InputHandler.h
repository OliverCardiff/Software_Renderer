#pragma once

namespace TakeOne
{
	class InputHandler
	{
	public:
		InputHandler(void);
		~InputHandler(void);

	public:
		void SetDown(int);
		void SetUp(int);
		void AllUp();
		Gdiplus::PointF* GetMouse();
		void SetMouse(POINT& p);
		bool IsDown(int key);
		Gdiplus::PointF * GetWindowPos();
		void SetWndPos(Gdiplus::PointF *);

	private:
		Gdiplus::PointF _mouse;
		Gdiplus::PointF * _window;
		bool _keys[256];
	};
}
