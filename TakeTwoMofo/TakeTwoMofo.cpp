// TakeTwoMofo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TakeTwoMofo.h"
#include "GameManager.h"

#define MAX_LOADSTRING 100
using namespace TakeOne;
using namespace Gdiplus;

const int WindowX = 800;
const int WindowY = 600;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
ULONG_PTR gdiplusStartupToken;					// gdi token
GameManager* game;								// controls rendering and game
void OnPaint(PAINTSTRUCT, HWND hWnd);	
POINT GetRelativeMouse(HWND hWnd, Gdiplus::PointF& _wndPos);
HDC hdcMem;
HBITMAP hbmMem, hbmOld;
bool created = false;
LPRECT lpRect = new RECT();	
LPRECT lpRect2 = new RECT();

void ClientResize(HWND hWnd, int nWidth, int nHeight); // resizes the window

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TAKETWOMOFO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TAKETWOMOFO));

	// Main message loop:
	while (true)
	{
		if(PeekMessage(&msg, NULL,0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT) break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			SendMessage(msg.hwnd, WM_INPUT,0,0);
			game->Update(msg.hwnd);
		}
		
	}
	GdiplusShutdown(gdiplusStartupToken);

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	GdiplusStartupInput gdiplusStartupInput;
	
	//initialize gdi
	GdiplusStartup(&gdiplusStartupToken, &gdiplusStartupInput, NULL);
	game = new GameManager();

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_OWNDC; 
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TAKETWOMOFO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TAKETWOMOFO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);


   if (!hWnd)
   {
      return FALSE;
   }
   ClientResize(hWnd, WindowX, WindowY);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdc;
	

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		hdcMem = CreateCompatibleDC(hdc);
		hbmMem = CreateCompatibleBitmap(hdc, WindowX, WindowY);

		ReleaseDC(hWnd, hdc);
		SelectObject(hdcMem, hbmMem);
		DeleteObject(hbmMem);

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		//case WM_ERASEBKGND:
           // return (LRESULT)1;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		game->GetMouseInput(GetRelativeMouse(hWnd , *game->_wndPos));
		game->Draw(hdcMem);
		ps.hdc = BeginPaint(hWnd, &ps);
		BitBlt(ps.hdc, 0, 0, WindowX, WindowY, hdcMem, 0, 0, SRCCOPY);
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	//case WM_ERASEBKGND:
        //return (LRESULT)1;
	case WM_KEYDOWN:
		game->GetInput((int)wParam, true);
		break;
	case WM_KEYUP:
		game->GetInput((int)wParam, false);
		break;
	case WM_LBUTTONDOWN:
		game->GetInput(32, true);
		break;
	case WM_LBUTTONUP:
		game->GetInput(32, false);
		break;
	case WM_MOUSEMOVE:
		game->GetMouseInput(GetRelativeMouse(hWnd , *game->_wndPos));
		break;
	case WM_SIZE:
		ClientResize(hWnd, WindowX, WindowY);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

	
}

 //Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void ClientResize(HWND hWnd, int nWidth, int nHeight)
{
  RECT rcClient, rcWindow;
  POINT ptDiff;
  GetClientRect(hWnd, &rcClient);
  GetWindowRect(hWnd, &rcWindow);
  ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
  ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
  MoveWindow(hWnd,rcWindow.left, rcWindow.top, nWidth + ptDiff.x, nHeight + ptDiff.y, FALSE);
}

void OnPaint(PAINTSTRUCT &ps, HWND & hWnd)
{
	
}
POINT GetRelativeMouse(HWND hWnd, Gdiplus::PointF& _wndPos)
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	int scY = 50;
	int scX = 50;

	GetWindowRect(hWnd, lpRect);
	GetClientRect(hWnd, lpRect2);	

	float x = (float)(lpRect->right - lpRect->left);
	float y = (float)(lpRect->bottom - lpRect->top);

	float diffX = x - lpRect2->right;
	float diffY = y - lpRect2->bottom;

	_wndPos.X = (Gdiplus::REAL)lpRect->left + diffX;
	_wndPos.Y = (Gdiplus::REAL)lpRect->top + diffY;
	
	if(true)
	{
		scY = (int)((cursorPos.y - lpRect->top) - diffY);
		scX = (int)((cursorPos.x - lpRect->left) - diffX);
	}

	if(scX < 0) scX = 0;
	if(scY < 0) scY = 0;

	cursorPos.y = scY;
	cursorPos.x = scX;

	return cursorPos;
}
