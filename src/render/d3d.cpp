#include "d3d.h"

d3dRenderer::d3dRenderer(HWND handle, int width, int height)
	: hwnd(handle), resWidth(width), resHeight(height),
	d3dDevice(nullptr), d3dImmediateContext(nullptr),
	dxgiSwapChain(nullptr), renderTargetView(nullptr),
	depthView(nullptr)
{}

d3dRenderer::~d3dRenderer()
{
	if (d3dDevice) {
		d3dDevice->Release();
	}

	if (d3dImmediateContext) {
		d3dImmediateContext->Release();
	}

	if (dxgiSwapChain) {
		dxgiSwapChain->Release();
	}

	if (renderTargetView) {
		renderTargetView->Release();
	}

	if (depthView) {
		depthView->Release();
	}
}

int d3dRenderer::InitializeRenderer()
{
	// Device Init
	D3D_FEATURE_LEVEL featureLevel;
	UINT flags = 0;
#if defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, NULL, 0, D3D11_SDK_VERSION, &d3dDevice, &featureLevel, &d3dImmediateContext);

	if (FAILED(hr)) {
		return -1;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		return -1;
	}

	if (InitializeSwapChain() < 0) {
		return -1;
	}

	// Render Target Init
	ID3D11Texture2D* backBuffer;
	hr = dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(hr)) {
		return -1;
	}

	hr = d3dDevice->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
	if (FAILED(hr)) {
		return -1;
	}
	backBuffer->Release();

	if (InitializeDepthStencilBuff() < 0) {
		return -1;
	}

	d3dImmediateContext->OMSetRenderTargets(1, &renderTargetView, depthView);
	if (FAILED(hr)) {
		return -1;
	}

	return 0;
}

int d3dRenderer::InitializeSwapChain()
{
	DXGI_SWAP_CHAIN_DESC1 sd = { 0 };
	sd.Width = resWidth;
	sd.Height = resHeight;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.Stereo = false;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.Scaling = DXGI_SCALING_STRETCH;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	sd.Flags = 0;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd = { 0 };
	fd.RefreshRate.Numerator = 60;
	fd.RefreshRate.Denominator = 1;

	fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fd.Scaling = DXGI_MODE_SCALING_STRETCHED;
	fd.Windowed = true;

	HRESULT hr;

	// Getting the Factory Instance
	IDXGIDevice* dxgiDevice = nullptr;
	if (FAILED(d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice))) {
		return -1;
	}

	IDXGIAdapter* dxgiAdapter = nullptr;
	if (FAILED(dxgiDevice->GetAdapter(&dxgiAdapter))) {
		return -1;
	}

	IDXGIFactory2* dxgiFactory = nullptr;
	if (FAILED(dxgiAdapter->GetParent(_uuidof(IDXGIFactory2), (void**)&dxgiFactory))) {
		return -1;
	}

	hr = dxgiFactory->CreateSwapChainForHwnd(d3dDevice, hwnd, &sd, &fd, NULL, &dxgiSwapChain);
	if (FAILED(hr)) {
		return -1;
	}

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	return 0;
}

int d3dRenderer::InitializeDepthStencilBuff()
{
	D3D11_TEXTURE2D_DESC td = { 0 };
	td.Width = 640;
	td.Height = 480;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D32_FLOAT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	HRESULT hr;

	ID3D11Texture2D* depthStencilBuffer = nullptr;
	hr = d3dDevice->CreateTexture2D(&td, 0, &depthStencilBuffer);
	if (FAILED(hr)) {
		return -1;
	}

	hr = d3dDevice->CreateDepthStencilView(depthStencilBuffer, 0, &depthView);
	if (FAILED(hr)) {
		return -1;
	}

	depthStencilBuffer->Release();

	return 0;
}