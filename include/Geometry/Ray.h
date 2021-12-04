#pragma once

#include "Geometry/Mesh.h"

struct RayHitData {
	bool Hit = false;
	class Mesh* Obj = nullptr;
	Trig* trig = nullptr;
	Vec3f HitPos;
};

class Ray
{
public:
	Vec3f Dir;
	Vec3f Pos;

	RayHitData HitData;

public:
	Ray(Vec3f& Dir, Vec3f& Pos);
	Ray();
	~Ray();
	void Cast(List<Mesh*>* objects, float ray_length);
};