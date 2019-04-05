#pragma once
#include "..\Entity.h"

class Plane : public Entity {
public:
	Plane(double x, double y, double z, Sprite* sprite, int initHealth);
	Plane(double x, double y, double z, Model * model, int initHealth);
	virtual ~Plane();

	int BehaviorLoop() override;

private:
	bool bSelected;
};
