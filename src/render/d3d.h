#pragma once

#include <dxgi1_6.h>
#include <d3d11_4.h>

class d3dRenderer
{
public:
	d3dRenderer(HWND handle, int width, int height);
	~d3dRenderer();
	int InitializeRenderer();

private:
	HWND hwnd;

	int resWidth;
	int resHeight;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dImmediateContext;
	IDXGISwapChain1* dxgiSwapChain;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthView;

	int InitializeSwapChain();
	int InitializeDepthStencilBuff();
};