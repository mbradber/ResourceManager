#include "DrawableTexture.h"

DrawableTexture::DrawableTexture(void):
	width(0), height(0), cmFormat(DXGI_FORMAT_UNKNOWN),
	pDevice(0), colorMapSRV(0), colorMapRTV(0) 
{
	ZeroMemory(&viewPort, sizeof(D3D10_VIEWPORT));
}


DrawableTexture::~DrawableTexture(void)
{
	colorMapSRV->Release();
	colorMapSRV = NULL;

	colorMapRTV->Release();
	colorMapRTV = NULL;
}

void DrawableTexture::init(ID3D10Device* device, UINT w, UINT h, DXGI_FORMAT colorFormat)
{
	pDevice = device;
	width = w;
	height = h;
	cmFormat = colorFormat;

	buildColorMap();

	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
}

void DrawableTexture::buildColorMap()
{
	ID3D10Texture2D* colorMap = 0;
	D3D10_TEXTURE2D_DESC texDesc;

	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = cmFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;
	texDesc.CPUAccessFlags = 0;
	pDevice->CreateTexture2D(&texDesc, 0, &colorMap);
	pDevice->CreateRenderTargetView(colorMap, 0, &colorMapRTV);
	pDevice->CreateShaderResourceView(colorMap, 0, &colorMapSRV);
}

void DrawableTexture::begin()
{
	ID3D10RenderTargetView* renderTargets[1] = {colorMapRTV};
	pDevice->OMSetRenderTargets(1, renderTargets, 0);
	pDevice->RSSetViewports(1, &viewPort);
	pDevice->ClearRenderTargetView(colorMapRTV, BLACK);
}

void DrawableTexture::end()
{
	pDevice->GenerateMips(colorMapSRV);
}
