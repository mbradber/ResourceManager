#include "Slider.h"
#include <dinput.h>

Slider::Slider()
{
	sliderPosX = 15;
	sliderPosY = 20;
	sliderWidth = 10;
	sliderHeight = 20;

	barPosX = 60;
	barPosY = 20;
	barWidth = 100;
	barHeight = 5;

	textPosX = 30;
	textPosY = 45;
	textWidth = 100;
	textHeight = 20;

	D3DXMatrixScaling(&sliderScale, sliderWidth, sliderHeight, 1.0f);
	D3DXMatrixScaling(&barScale, barWidth, barHeight, 1.0f);
	D3DXMatrixScaling(&textScale, textWidth, textHeight, 1.0f);

	D3DXMatrixTranslation(&barTranslation, barPosX, barPosY, 0.1f);
	barObject.matWorld = barScale * barTranslation;

	D3DXMatrixTranslation(&textTranslation, textPosX, textPosY, 0.1f);
	textObject.matWorld = textScale * textTranslation;
}

Slider::~Slider()
{
	FREE(sliderSprite);
	FREE(sliderObject.pTexture);
	FREE(barObject.pTexture);
	FREE(textObject.pTexture);
}

void Slider::initSprite(ID3D10Device* device)
{
	pDevice = device;

	createObject(L"Images//slider.png", &sliderObject, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1));
	createObject(L"Images//bar.png", &barObject, RED);
	createObject(L"Images//cameraSpeed.png", &textObject, BLACK);
	D3DX10CreateSprite(pDevice, 0, &sliderSprite);
}

void Slider::createObject(LPCWSTR filename, D3DX10_SPRITE* spriteObject, D3DXCOLOR color)
{
	ID3D10Resource* spriteResource;
	ID3D10Texture2D* spriteTexture;
	ID3D10ShaderResourceView* spriteSRV;

	HRESULT hr = D3DX10CreateTextureFromFile(pDevice, filename, 0, 0, &spriteResource, 0);
	if(FAILED(hr))
		MessageBoxA(0, "An image file is missing", 0, 0); 

	spriteResource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&spriteTexture);

	D3D10_TEXTURE2D_DESC desc;
	spriteTexture->GetDesc(&desc);

	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	pDevice->CreateShaderResourceView(spriteTexture, &srvDesc, &spriteSRV);

	spriteObject->pTexture = spriteSRV;
	spriteObject->TexCoord.x = 0;
	spriteObject->TexCoord.y = 0;
	spriteObject->TexSize.x = 1;
	spriteObject->TexSize.y = 1;
	spriteObject->TextureIndex = 0;
	spriteObject->ColorModulate = color;

	//Release COM objects used in this function
	FREE(spriteResource);
	FREE(spriteTexture);
}

void Slider::update(BYTE* keyboard, float delta)
{
	if(sliderPosX < 1)
		sliderPosX = 1;

	if(keyboard[DIK_RIGHT] & 0x80 && sliderPosX < 99)
		sliderPosX += delta * 200;
	if(keyboard[DIK_LEFT] & 0x80 && sliderPosX > 1)
		sliderPosX -= delta * 200;

	D3DXMatrixTranslation(&sliderTranslation, sliderPosX + 10, sliderPosY, 0.1f);

	sliderObject.matWorld = sliderScale * sliderTranslation;
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
	sliderSprite->DrawSpritesImmediate(&sliderObject, 1, 0, 0);
	sliderSprite->DrawSpritesImmediate(&barObject, 1, 0, 0);
	sliderSprite->DrawSpritesImmediate(&textObject, 1, 0, 0);
	sliderSprite->End();
}
