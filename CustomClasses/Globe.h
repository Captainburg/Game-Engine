#pragma once
#include "..\Entity.h"

class Globe : public Entity {
public:
	Globe(double x, double y, double z, Sprite* sprite, int initHealth);
	Globe(double x, double y, double z, Model * model, int initHealth);
	virtual ~Globe();

	int BehaviorLoop() override;

private:
	bool bSelected;
};