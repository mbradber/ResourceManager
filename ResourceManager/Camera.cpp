#include "Camera.h"
#include<cstring>
#include<cmath>

Camera::Camera():
maxPitch(PI / 4), currentPitch(0)
{
}

void Camera::init(D3DXVECTOR3 pos, D3DXVECTOR3 target, D3DXVECTOR3 up)
{
	cameraPosition = pos;
	cameraTarget = target;
	cameraUp = up;
	cameraDirection = cameraTarget - cameraPosition;
	D3DXVec3Normalize(&cameraDirection, &cameraDirection);
	D3DXVec3Cross(&cameraSide, &cameraUp, &cameraDirection);
}

void Camera::Update(BYTE* keyboard, DIMOUSESTATE mouse, float delta)
{
	memcpy(keystate, keyboard, 256);
	mouseState = mouse;
	deltaTime = delta;

	//handle key input
	//MOVEMENTS ALONG Z AXIX
	if(keystate[DIK_W] & 0x80)
		cameraPosition += cameraDirection * deltaTime * velocity;
	if(keystate[DIK_S] & 0x80)
		cameraPosition -= cameraDirection * deltaTime * velocity;
	//MOVEMENTS ALONG X AXIS
	if(keystate[DIK_A] & 0x80)
		cameraPosition -= cameraSide * deltaTime * velocity;
	if(keystate[DIK_D] & 0x80)
		cameraPosition += cameraSide * deltaTime * velocity;
	//MOVEMENTS ALONG Y AXIS
	if(keystate[DIK_Z] & 0x80)
		cameraPosition += cameraUp * deltaTime * velocity;
	if(keystate[DIK_X] & 0x80)
		cameraPosition -= cameraUp * deltaTime * velocity;

	//enable camera rotations from mouse
	if(mouseState.rgbButtons[0] & 0x80)//check that left mouse is down
	{
		//CAMERA YAW(unrestricted)
		D3DXMATRIX rotateYaw;
		D3DXMatrixRotationAxis(&rotateYaw, &cameraUp, mouseState.lX * 0.005f);
		D3DXVECTOR4 cameraDirectionTemp = D3DXVECTOR4(cameraDirection.x, cameraDirection.y, cameraDirection.z, 0);
		D3DXVec4Transform(&cameraDirectionTemp, &cameraDirectionTemp, &rotateYaw);
		cameraDirection = D3DXVECTOR3(cameraDirectionTemp.x, cameraDirectionTemp.y, cameraDirectionTemp.z);

		//CAMERA PITCH(restricted field of view)
		float pitchAngle = mouseState.lY * 0.005f;

		if(abs(currentPitch + pitchAngle) < maxPitch)
		{
			D3DXVec3Cross(&cameraSide, &cameraUp, &cameraDirection);
			D3DXMATRIX rotatePitch;
			D3DXMatrixRotationAxis(&rotatePitch, &cameraSide, pitchAngle);
			cameraDirectionTemp = D3DXVECTOR4(cameraDirection.x, cameraDirection.y, cameraDirection.z, 0);
			D3DXVec4Transform(&cameraDirectionTemp, &cameraDirectionTemp, &rotatePitch);
			cameraDirection = D3DXVECTOR3(cameraDirectionTemp.x, cameraDirectionTemp.y, cameraDirectionTemp.z);

			currentPitch += pitchAngle;
		}
	}

	//rebuild the camera target point
	cameraTarget = cameraPosition + cameraDirection;
	//build the camera view matrix
	D3DXMatrixLookAtLH(&cameraViewMatrix, &cameraPosition, &cameraTarget, &cameraUp);
}

