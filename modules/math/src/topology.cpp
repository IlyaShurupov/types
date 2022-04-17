
#include "topology.h"

#define MATH_EPSILON 0.00001

vec3f indexed_trig::HitPos;

indexed_trig::indexed_trig() : v1(0), v2(0), v3(0) {}
indexed_trig::indexed_trig(halni v1, halni v2, halni v3) : v1(v1), v2(v2), v3(v3) {}

void indexed_trig::update_cache(Array<vec3f>& points) {
	v1_val = points[v1];
	edge1 = points[v2] - points[v1];
	edge2 = points[v3] - points[v1];
	normal = (points[v2] - points[v1]).Cross(points[v3] - points[v1]).Dir();
}

bool indexed_trig::RayHit(const Ray& ray) {
	static vec3f h, s, q;
	static halnf a, f, u, v;
	static halnf t;

	h = ray.Dir.Cross(edge2);
	a = edge1.Dot(h);

	if (a > -MATH_EPSILON && a < MATH_EPSILON) {
		return false;
	}

	f = 1.f / a;
	s = ray.Pos - v1_val;
	u = f * s.Dot(h);

	if (u < 0.0 || u > 1.0) {
		return false;
	}

	q = s.Cross(edge1);
	v = f * ray.Dir.Dot(q);

	if (v < 0.f || u + v > 1.f) {
		return false;
	}

	t = f * edge2.Dot(q);
	if (t > MATH_EPSILON) {
		HitPos = ray.Pos + ray.Dir * t;
		return true;
	} else {
		return false;
	}
}

vec3f& indexed_trig::get_hit_pos() {
	return HitPos;
}

vec3f indexed_trig::get_normal() {
	return normal;
}

void topology::add_trig(vec3f v1, vec3f v2, vec3f v3) {

	halni trig_idx = points.Len();
	Array<vec3f> newpoints(3);
	newpoints[0] = v1;
	newpoints[1] = v2;
	newpoints[2] = v3;

	points += newpoints;

	transform_point(newpoints[0]);
	transform_point(newpoints[1]);
	transform_point(newpoints[2]);

	transformed_points += newpoints;

	trig newtrig(trig_idx, trig_idx + 1, trig_idx + 2);
	newtrig.update_cache(transformed_points);
	trigs.PushBack(newtrig);
}

void topology::transform_point(vec3f& vert) {
	basis.Transform(vert);
	vert += origin;
}

void topology::update_transformed() {
	transformed_points = points;
	for (alni idx : range(transformed_points.Len())) {
		transform_point(transformed_points[idx]);
	}
	for (alni idx : range(trigs.Len())) {
		trigs[idx].update_cache(transformed_points);
	}
}