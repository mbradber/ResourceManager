#include "Slider.h"
#include <dinput.h>

Slider::Slider()
{
	sliderPosX = 10;
	sliderPosY = 20;
	sliderWidth = 10;
	sliderHeight = 20;

	D3DXMatrixScaling(&sliderScale, sliderWidth, sliderHeight, 1.0f);
}

Slider::~Slider()
{
	if(sliderSprite)
		sliderSprite->Release();
	if(spriteObject.pTexture)
		spriteObject.pTexture->Release();
}

void Slider::initSprite(ID3D10Device* device)
{
	pDevice = device;

	ID3D10Resource* sliderResource;
	ID3D10Texture2D* sliderTexture;
	ID3D10ShaderResourceView* sliderSRV;

	HRESULT hr = D3DX10CreateTextureFromFile(pDevice, L"slider.png", 0, 0, &sliderResource, 0);
	if(FAILED(hr))
		MessageBoxA(0, "Image file \'slider.png\' not found", 0, 0); 

	sliderResource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&sliderTexture);

	D3D10_TEXTURE2D_DESC desc;
	sliderTexture->GetDesc(&desc);

	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	pDevice->CreateShaderResourceView(sliderTexture, &srvDesc, &sliderSRV);

	spriteObject.pTexture = sliderSRV;
	spriteObject.TexCoord.x = 0;
	spriteObject.TexCoord.y = 0;
	spriteObject.TexSize.x = 1;
	spriteObject.TexSize.y = 1;
	spriteObject.TextureIndex = 0;
	spriteObject.ColorModulate = WHITE;

	D3DX10CreateSprite(pDevice, 0, &sliderSprite);

	//Release COM objects used in this function
	if(sliderResource)
	{
		sliderResource->Release();
		sliderResource = NULL;
	}

	if(sliderTexture)
	{
		sliderTexture->Release();
		sliderTexture = NULL;
	}
}

void Slider::update(BYTE* keyboard, float delta)
{
	if(sliderPosX < 10)
		sliderPosX = 10;

	if(keyboard[DIK_RIGHT] & 0x80 && sliderPosX < 110)
		sliderPosX += delta * 200;
	if(keyboard[DIK_LEFT] & 0x80 && sliderPosX > 10)
		sliderPosX -= delta * 200;

	D3DXMatrixTranslation(&sliderTranslation, sliderPosX, sliderPosY, 0.1f);

	spriteObject.matWorld = sliderScale * sliderTranslation;
}

void Slider::rebuildProjection(int w, int h)
{
	D3DXMATRIX spriteProjectionMatrix;
	D3DXMatrixOrthoOffCenterLH(&spriteProjectionMatrix, 0, w, 0, h, 0.1f, 10);
	sliderSprite->SetProjectionTransform(&spriteProjectionMatrix);
}

void Slider::draw()
{
	sliderSprite->Begin(D3DX10_SPRITE_SORT_TEXTURE);
	sliderSprite->DrawSpritesImmediate(&spriteObject, 1, 0, 0);
	sliderSprite->End();
}
