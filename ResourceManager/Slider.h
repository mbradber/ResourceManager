#pragma once
#include"Utility.h"

class Slider
{
public:
	Slider();
	~Slider(void);
	void initSprite(ID3D10Device* device);
	void update(BYTE* keyboard, float delta);
	void update(long movement);
	void rebuildProjection(int w, int h);
	void draw();

	float sliderPosX, sliderPosY, sliderWidth, sliderHeight;
private:
	void createObject(LPCWSTR filename, D3DX10_SPRITE* spriteObject, D3DXCOLOR);
	ID3D10Device* pDevice;
	ID3DX10Sprite* sliderSprite;

	D3DX10_SPRITE sliderObject, barObject, textObject;

	float barPosX, barPosY, barWidth, barHeight, textPosX, textPosY, textWidth, textHeight;

	D3DXMATRIX sliderTranslation, sliderScale, barTranslation, barScale, 
		textTranslation, textScale;
};



