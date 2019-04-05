#pragma once
#include "..\Entity.h"

class Tiger: public Entity {
public:
	Tiger(double x, double y, double z, Sprite* sprite, int initHealth);
	Tiger(double x, double y, double z, Model * model, int initHealth);
	virtual ~Tiger();

	int BehaviorLoop() override;

private:
	bool bSelected;
};