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

inline float Mirror::px()
{
	return planex;
}

inline float Mirror::py()
{
	return planey;
}

inline float Mirror::pz()
{
	return planez;
}

inline float Mirror::mx()
{
	return mirrorx;
}

inline float Mirror::my()
{
	return mirrory;
}

inline float Mirror::mz()
{
	return mirrorz;
}

inline unsigned int Mirror::rN()
{
	return rectNum;
}