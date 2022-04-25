#pragma once

#include "vec.h"

class Trig
{
public:
	Trig();
	Trig(const vec3f& v0, const vec3f& v1, const vec3f& v2);
	~Trig();

	vec3f V0;
	vec3f V1;
	vec3f V2;

	void assign(const vec3f& v0, const vec3f& v1, const vec3f& v2);
	void Normal(vec3f& dir);
	bool RayHit(class Ray& ray, vec3f& HitPos);
};
