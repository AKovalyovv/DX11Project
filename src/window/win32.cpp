#include "win32.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND GenerateWindow(const wchar_t* windowName, HINSTANCE hInstance, int resWidth, int resHeight)
{

	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = windowName;
	
	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0,
		windowName,
		windowName,
		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT, CW_USEDEFAULT, resWidth, resHeight,

		NULL,
		NULL,
		hInstance,
		NULL
	);

	return hwnd;
}