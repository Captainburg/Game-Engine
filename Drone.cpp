#include "Drone.h"

Drone::Drone(double x, double y, double z, Sprite * sprite, int initHealth)
	: Entity(x, y, z, sprite, initHealth)
	, bSelected(true)
{
	mYaw = D3DX_PI * 0.2;
}

Drone::Drone(double x, double y, double z, Model * model, int initHealth)
	: Entity(x, y, z, model, initHealth)
	, bSelected(true)
{
	mYaw = D3DX_PI * 0.2;
}

Drone::~Drone()
{
}

int Drone::BehaviorLoop()
{
	//W Forward
	if (GetAsyncKeyState(0x57))
	{
		mZ -= 0.05;
	}
	//A Left
	if (GetAsyncKeyState(0x41))
	{
		mX += 0.05;
	}
	//D Right
	if (GetAsyncKeyState(0x44))
	{
		mX -= 0.05;
	}
	//S Backward
	if (GetAsyncKeyState(0x53))
	{
		mZ += 0.05;
	}
	//Shift Downward
	if (GetAsyncKeyState(VK_SHIFT))
	{
		mY -= 0.05;
	}
	//Space Upward
	if (GetAsyncKeyState(VK_SPACE))
	{
		mY += 0.05;
	}
	//Num7 CCW - X
	if (GetAsyncKeyState(VK_NUMPAD7))
	{
		mPitch -= 0.03;
	}
	//Num9 CW - X
	if (GetAsyncKeyState(VK_NUMPAD9))
	{
		mPitch += 0.03;
	}
	//Num4 CCW - Y
	if (GetAsyncKeyState(VK_NUMPAD4))
	{
		mYaw -= 0.03;
	}
	//Num6 CW - Y
	if (GetAsyncKeyState(VK_NUMPAD6))
	{
		mYaw += 0.03;
	}
	//Num1 CCW - Z
	if (GetAsyncKeyState(VK_NUMPAD1))
	{
		mRoll -= 0.03;
	}
	//Num3 CW - Z
	if (GetAsyncKeyState(VK_NUMPAD3))
	{
		mRoll += 0.03;
	}

	GetModel()->GetBSphere()->_center = D3DXVECTOR3(GetX(), GetY(), GetZ());
	return 0;
}
