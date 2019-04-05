#pragma once
#include "..\Entity.h"

class Drone : public Entity {
public:
	Drone(double x, double y, double z, Sprite* sprite, int initHealth);
	Drone(double x, double y, double z, Model * model, int initHealth);
	virtual ~Drone();

	int BehaviorLoop() override;

private:
	bool bSelected;
};