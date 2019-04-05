#include "Mirror.h"

Mirror::Mirror(float px, float py, float pz, float mx, float my, float mz, unsigned int rN)
	: planex(px)
	, planey(py)
	, planez(pz)
	, mirrorx(mx)
	, mirrory(my)
	, mirrorz(mz)
	, rectNum(rN)
{
}

Mirror::~Mirror()
{
}

float Mirror::px()
{
	return planex;
}

float Mirror::py()
{
	return planey;
}

float Mirror::pz()
{
	return planez;
}

float Mirror::mx()
{
	return mirrorx;
}

float Mirror::my()
{
	return mirrory;
}

float Mirror::mz()
{
	return mirrorz;
}

unsigned int Mirror::rN()
{
	return rectNum;
}