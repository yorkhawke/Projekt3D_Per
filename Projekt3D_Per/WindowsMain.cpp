#include "WindowsMain.h"

void WindowsMain::CreateHwnd(HINSTANCE hInstance,WNDPROC WndProc)
{
	MSG msg = { 0 };
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"Pro Window";
	wcex.hCursor = LoadCursor(NULL, IDC_CROSS);

	if (RegisterClassEx(&wcex))
	{
		RECT rc = { 0, 0, ScreenWidth, ScreenHeight };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		window = CreateWindow(
			L"Pro Window",
			L"Mah Game",
			WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rc.right - rc.left,
			rc.bottom - rc.top,
			nullptr, //parent
			nullptr,
			hInstance,
			nullptr);
	}
}
