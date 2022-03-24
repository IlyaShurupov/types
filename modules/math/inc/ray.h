#pragma once

#include "mesh.h"

#include "list.h"

struct RayHitData {
	bool Hit = false;
	class Mesh* Obj = nullptr;
	Trig* trig = nullptr;
	vec3f HitPos;
};

class Ray
{
public:
	vec3f Dir;
	vec3f Pos;

	RayHitData HitData;

public:
	Ray(vec3f& Dir, vec3f& Pos);
	Ray();
	~Ray();
	void Cast(list<Mesh*>* objects, float ray_length);
};