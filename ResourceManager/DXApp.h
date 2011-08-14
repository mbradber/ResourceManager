#pragma once
#include"Utility.h"
#include"Timer.h"
#include<string>
#include<sstream>
#include<dinput.h>

class DXApp
{
public:
	DXApp();
	virtual ~DXApp();

	virtual void handleResize();
	virtual LRESULT cbProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	virtual void updateScene(float delta);
	virtual void draw();
	virtual bool initWindow(HINSTANCE instanceHandle, int show);
	virtual void initD3D();
	virtual void detectInput();
	void resetTargets();

	void initDInput(HINSTANCE hi, HWND hWnd);
	HWND getMainWindow(){return mainWindow;}
	int run();

protected:
	int CLIENT_WIDTH;
	int CLIENT_HEIGHT;
	HWND mainWindow;

	ID3D10Device * mDevice;
	IDXGISwapChain * mSwapChain;
	D3D10_VIEWPORT vp;

	ID3D10RenderTargetView * rtView;
	ID3D10DepthStencilView * dsView;
	ID3D10Texture2D * dsBuffer;
	ID3DX10Font * font;

	D3DXCOLOR mClearColor;
	D3DXCOLOR fontColor;
	RECT someRect;

	//DirectInput variables
	LPDIRECTINPUT8 din;
	LPDIRECTINPUTDEVICE8 dinKeyboard;
	LPDIRECTINPUTDEVICE8 dinMouse;
	BYTE keystate[256];
	DIMOUSESTATE mouseState;

	//Time variables
	Timer gameTimer;
	std::wstring totalTime;
	int frames;
	int fps;
	double mpf;

	bool resizing;
	__int64 deltaA;
	__int64 deltaB;
	float deltaTime;

	__int64 cps;

private:
	//Mouse location variables
	POINT findRelativeMousePos();
	POINT points[1];
	RECT clientLoc[1];

};

