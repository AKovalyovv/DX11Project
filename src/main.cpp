#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "window/win32.h"
#include "render/d3d.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	int resWidth = 640;
	int resHeight = 480;

	HWND hwnd = GenerateWindow(L"Test Window", hInstance, resWidth, resHeight);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	d3dRenderer renderer(hwnd, resWidth, resHeight);

	if (renderer.InitializeRenderer() < 0) {
		return -1;
	}

	MSG msg = { };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}