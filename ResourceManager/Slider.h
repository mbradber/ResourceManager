#pragma once
#include"Utility.h"

class Slider
{
public:
	Slider();
	~Slider(void);
	void initSprite(ID3D10Device* device);
	void update(BYTE* keyboard, float delta);
	void rebuildProjection(int w, int h);
	void draw();
	float getPosX() {return sliderPosX - 9;}

private:
	ID3D10Device* pDevice;
	ID3DX10Sprite* sliderSprite;
	D3DX10_SPRITE spriteObject;
	float sliderPosX, sliderPosY, sliderWidth, sliderHeight;
	D3DXMATRIX sliderTranslation, sliderScale;
};



