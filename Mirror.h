#pragma once

class Mirror {
public:
	Mirror(float px, float py, float pz, float mx, float my, float mz, unsigned int rN);
	virtual ~Mirror();

	float px();
	float py();
	float pz();

	float mx();
	float my();
	float mz();

	unsigned int rN();

private:
	float planex;
	float planey;
	float planez;

	float mirrorx;
	float mirrory;
	float mirrorz;

	unsigned int rectNum;
};