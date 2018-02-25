#pragma once
#include "Sprite.h"
#include "Model.h"

class Entity {
public:
	Entity(int x, int y, int z, Sprite* sprite, int initHealth);
	Entity(int x, int y, int z, Model * model, int initHealth);
	virtual ~Entity();


	/** Is Entity Alive
	*
	* Checks if the entity is alive.
	*
	* @return bool true if alive, false if inanimate.
	*/
	bool IsAlive() const;
	/** SetHealth
	*
	* Sets the health of this Entity if it is alive.
	*
	* @param health The desired current health of the entity.
	* @param maxHealth The desired maximum health of the entity
	* @return null
	*/
	void SetHealth(int health, int maxHealth);
	/** Take Damage
	*
	* Makes this entity take the specified amount of damage.
	*
	* @param damage The amount of damage this object is going to be dealt.
	* @return null
	*/
	void TakeDamage(int damage);
	/** Restore Health
	*
	* Makes this entity recieve the specified amount of health.
	*
	* @param heal The amount of health this object is going to have restored.
	* @return null
	*/
	void RestoreHealth(int heal);
	/** Get Health
	*
	* Returns the current health of this entity.
	*
	* @return int The amount of health remaining.
	*/
	int GetHealth() const;

	/** Is Drawable
	*
	* Returns whether this entity can be drawn.
	*
	* @return bool Whether this entity can be drawn.
	*/
	bool IsDrawable2D() const;
	/** Is Drawable
	*
	* Returns whether this entity can be drawn.
	*
	* @return bool Whether this entity can be drawn.
	*/
	bool IsDrawable3D() const;
	/** Get Sprite
	*
	* Returns this Entities Sprite pointer.
	*
	* @return Sprite* this Entities Sprite pointer.
	*/
	Sprite* GetSprite() const;

	/** Move To (x, y, z)
	*
	* Moves this Entity.
	*
	* @param x The number that mX will be set to.
	* @param y The number that mY will be set to.
	* @param z The number that mZ will be set to.
	* @return null
	*/
	void MoveTo(int x, int y, int z);
	/** Move To (x, y)
	*
	* Moves this Entity.
	*
	* @param x The number that mX will be set to.
	* @param y The number that mY will be set to.
	* @return null
	*/
	void MoveTo(int x, int y);

	/** Get X
	*
	* Returns the value of the X co-ordinate.
	*
	* @return int the value of the X co-ordinate
	*/
	int GetX();
	/** Get Y
	*
	* Returns the value of the Y co-ordinate.
	*
	* @return int the value of the Y co-ordinate
	*/
	int GetY();
	/** Get Z
	*
	* Returns the value of the Z co-ordinate.
	*
	* @return int the value of the Z co-ordinate
	*/
	int GetZ();

	/** Behavior Loop
	*
	* To be overwritten by child classes, and called by the GameLoop().
	*
	* @return null
	*/
	virtual int BehaviorLoop();

private:
	bool bAlive;
	int mMaxHealth;
	int mHealth;

	bool bDrawable;
	Sprite* mSprite;
	Model* mModel;

	bool b3D;
	int mX;
	int mY;
	int mZ;
};