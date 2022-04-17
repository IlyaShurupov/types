#pragma once

#include "array.h"
#include "npple.h"
#include "mat3.h"
#include "Ray.h"

class indexed_trig {
	public:

	halni v1, v2, v3;

	vec3f edge1;
	vec3f edge2;
	vec3f v1_val;
	vec3f normal;

	static vec3f HitPos;

	indexed_trig();
	indexed_trig(halni v1, halni v2, halni v3);
	void update_cache(Array<vec3f>& points);
	bool RayHit(const Ray& ray);
	vec3f& get_hit_pos();
	vec3f get_normal();
};

class topology {
	public:

	mat3f basis;
	vec3f origin;

	Array<vec3f> points;
	Array<vec3f> transformed_points;

	typedef indexed_trig trig;
	Array<trig> trigs;
	
	void transform_point(vec3f& vert);

	topology() {}
	void add_trig(vec3f v1, vec3f v2, vec3f v3);
	void update_transformed();
	~topology() {}
};