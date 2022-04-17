#pragma once

#include "vec3.h"

class Ray {
	public:
	vec3f Dir;
	vec3f Pos;

	public:
	Ray(const vec3f& Dir, const  vec3f& Pos);
	Ray();
	~Ray();
};