#pragma once

#include "array.h"
#include "npple.h"
#include "mat.h"
#include "vec.h"
#include "Ray.h"

struct camera {

	mat3f tmat;
	vec3f pos;
	halnf fov = (halnf) (PI) / 4;
	halnf ratio = 1.f;
	halnf near = 1.f;
	halnf far = 100.f;

	halnf targetlnegth = 1.f;
	bool updated = false;

	camera() {
		lookat(vec3f(0, 0, 0), vec3f(1, 0, 0), vec3f(0, 0, 1));
	}

	void set_ratio(halnf ratio);
	void set_fov(halnf fov);

	vec3f get_target();
	void lookat(vec3f target, vec3f pos, vec3f up);

	vec3f project(vec2f normalized);
	vec2f project(vec3f world);

	mat<halnf, 4, 4> projmat();
	mat<halnf, 4, 4> viewmat();

	void rotate(halnf anglex, halnf angley);
	void move(vec2f pos, vec2f prevpos);
	void zoom(halnf ratio);
};

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