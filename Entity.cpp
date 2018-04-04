#include "Entity.h"

Entity::Entity(double x, double y, double z, Sprite* sprite, int initHealth)
	: mX(x)
	, mY(y)
	, mZ(z)
	, mPitch(0)
	, mYaw(0)
	, mRoll(0)
	, mSprite(sprite)
	, mHealth(initHealth)
	, mMaxHealth(initHealth)
{
	//Sprite Related
	b3D = false;
	if (sprite == nullptr) {
		bDrawable = false;
	}
	else {
		bDrawable = true;
	}

	//Logic Related
	if (initHealth == 0) {
		bAlive = false;
	}
}

Entity::Entity(double x, double y, double z, Model* model, int initHealth)
	: mX(x)
	, mY(y)
	, mZ(z)
	, mPitch(0)
	, mYaw(0)
	, mRoll(0)
	, mModel(model)
	, mHealth(initHealth)
	, mMaxHealth(initHealth)
{
	//Model Related
	if (model == nullptr) {
		bDrawable = false;
		b3D = false;
	}
	else {
		bDrawable = true;
		b3D = true;
		GetModel()->GetBSphere()->_center = D3DXVECTOR3(GetX(), GetY(), GetZ());
	}

	//Logic Related
	if (initHealth == 0) {
		bAlive = false;
	}
}

Entity::~Entity()
{
	if (IsDrawable2D()) {
		delete mSprite;
	}
	if (IsDrawable3D()) {
		delete mModel;
	}
}

void Entity::SetupMatrices(LPDIRECT3DDEVICE9 g_pDevice, double cam_x, double cam_y, double cam_z, double cam_spin, double cam_angle) {

	//Net Transform
	D3DXMATRIXA16 matWorld_Net;

	//Get Location
	D3DXMATRIXA16 matWorld_XYZ;
	D3DXMatrixTranslation(&matWorld_XYZ, mX, mY, mZ);

	//Get Rotation
	D3DXMATRIXA16 matWorld_YPR;
	D3DXMatrixRotationYawPitchRoll(&matWorld_YPR, mYaw, mPitch, mRoll);

	//Get Net Transform
	D3DXMatrixMultiply(&matWorld_Net, &matWorld_YPR, &matWorld_XYZ);

	g_pDevice->SetTransform(D3DTS_WORLD, &matWorld_Net);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the 
	// origin, and define "up" to be in the y-direction.
	double yWeakness = 1 - pow(fabs(cam_angle - 0.5) * 2, 2);

	D3DXVECTOR3 vEyePt(cam_x, cam_y, cam_z);
	D3DXVECTOR3 vLookatPt(cam_x + sin(cam_spin * D3DX_PI * 2) * yWeakness, cam_y + cos(cam_angle * D3DX_PI), cam_z + sin((cam_spin - 0.25) * D3DX_PI * 2) * yWeakness);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

bool Entity::IsAlive() const
{
	return bAlive;
}

void Entity::SetHealth(int health, int maxHealth)
{
	if (!IsAlive())
	{
		return;
	}
	mHealth = health;
	mMaxHealth = maxHealth;

	if (mHealth > mMaxHealth)
	{
		mHealth = mMaxHealth;
	}
}

void Entity::TakeDamage(int damage)
{
	if (!IsAlive())
	{
		return;
	}
	mHealth -= damage;

	if (mHealth < 0)
	{
		mHealth = 0;
	}
}

void Entity::RestoreHealth(int heal)
{
	if (!IsAlive())
	{
		return;
	}
	mHealth += heal;

	if (mHealth > mMaxHealth)
	{
		mHealth = mMaxHealth;
	}
}

int Entity::GetHealth() const
{
	return mHealth;
}

bool Entity::IsDrawable2D() const
{
	return bDrawable && !b3D;
}

bool Entity::IsDrawable3D() const
{
	return bDrawable && b3D;
}

Sprite* Entity::GetSprite() const
{
	return mSprite;
}

Model* Entity::GetModel() const
{
	return mModel;
}

void Entity::MoveTo(double x, double y, double z)
{
	mX = x;
	mY = y;
	mZ = z;
}

double Entity::GetX()
{
	return mX;
}

double Entity::GetY()
{
	return mY;
}

double Entity::GetZ()
{
	return mZ;
}

int Entity::BehaviorLoop()
{
	return 0;
}
