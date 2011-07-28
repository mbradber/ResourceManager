#pragma once
#include"Utility.h"

class DrawableTexture
{
public:
	DrawableTexture(void);
	~DrawableTexture(void);

	void init(ID3D10Device* device, UINT w, UINT h, DXGI_FORMAT colorFormat);
	void begin();
	void end();

	ID3D10ShaderResourceView* colorMapSRV;
	ID3D10RenderTargetView* colorMapRTV;

private:
	void buildColorMap();

	UINT width;
	UINT height;
	DXGI_FORMAT cmFormat;
	ID3D10Device* pDevice;
	D3D10_VIEWPORT viewPort;
};

