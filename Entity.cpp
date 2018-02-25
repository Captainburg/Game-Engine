#include "Entity.h"

Entity::Entity(int x, int y, int z, Sprite* sprite, int initHealth)
	: mX(x)
	, mY(y)
	, mZ(z)
	, mSprite(sprite)
	, mHealth(initHealth)
	, mMaxHealth(initHealth)
{
	if (sprite == nullptr) {
		bDrawable = false;
	}
	if (initHealth == 0) {
		bAlive = false;
	}
}

Entity::~Entity()
{
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

bool Entity::IsDrawable() const
{
	return bDrawable;
}

Sprite* Entity::GetSprite() const
{
	return mSprite;
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
