#pragma once

#include "Vec3.h"

class Trig
{
public:
	Trig();
	Trig(Vec3f& v0, Vec3f& v1, Vec3f& v2);
	~Trig();

	Vec3f V0;
	Vec3f V1;
	Vec3f V2;

	void Normal(Vec3f& dir);
	bool RayHit(class Ray& ray, Vec3f& HitPos);
};
