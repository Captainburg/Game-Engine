#include "Entity.h"

Entity::Entity(int x, int y, int z, Sprite* sprite, int initHealth)
	: mX(x)
	, mY(y)
	, mZ(z)
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

Entity::Entity(int x, int y, int z, Model* model, int initHealth)
	: mX(x)
	, mY(y)
	, mZ(z)
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

void Entity::MoveTo(int x, int y, int z)
{
	mX = x;
	mY = y;
	mZ = z;
}

void Entity::MoveTo(int x, int y)
{
	mX = x;
	mY = y;
}

int Entity::GetX()
{
	return mX;
}

int Entity::GetY()
{
	return mY;
}

int Entity::GetZ()
{
	return mZ;
}

int Entity::BehaviorLoop()
{
	return 0;
}
