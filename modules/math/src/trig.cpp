#include "trig.h"
#include "ray.h"

#define MATH_EPSILON 0.00001

Trig::Trig() {
	V0.assign(0.f, 0.f, 0.f);
	V1.assign(0.f, 0.f, 0.f);
	V2.assign(0.f, 0.f, 0.f);
}

Trig::Trig(const vec3f& v0, const vec3f& v1, const vec3f& v2) {
	V0 = v0;
	V1 = v1;
	V2 = v2;
}

void Trig::assign(const vec3f& v0, const vec3f& v1, const vec3f& v2) {
	V0 = v0;
	V1 = v1;
	V2 = v2;
}

Trig::~Trig() {
}

void Trig::Normal(vec3f& dir)
{
	dir = (V1 - V0).Cross(V2 - V0);
	dir.Normalize();
}

vec3f edge1, edge2, h, s, q;
halnf a, f, u, v;
halnf t;

bool Trig::RayHit(class Ray& ray, vec3f& HitPos) {

	edge1 = V1 - V0;
	edge2 = V2 - V0;

	h = ray.Dir.Cross(edge2);
	a = edge1.Dot(h);

	if (a > -MATH_EPSILON && a < MATH_EPSILON) {
		return false;
	}

	f = 1.f / a;
	s = ray.Pos - V0;
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
	}
	else {
		return false;
	}
}