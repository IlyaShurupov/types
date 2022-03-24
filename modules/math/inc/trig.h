#pragma once

#include "vec3.h"

class Trig
{
public:
	Trig();
	Trig(vec3f& v0, vec3f& v1, vec3f& v2);
	~Trig();

	vec3f V0;
	vec3f V1;
	vec3f V2;

	void Normal(vec3f& dir);
	bool RayHit(class Ray& ray, vec3f& HitPos);
};
