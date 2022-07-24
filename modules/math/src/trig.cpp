#include "trig.h"
#include "ray.h"

#define MATH_EPSILON 0.00001

namespace tp {

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

	Trig::~Trig() {}

	void Trig::normal(vec3f& dir) {
		dir = (V1 - V0).cross(V2 - V0);
		dir.normalize();
	}

	vec3f edge1, edge2, h, s, q;
	halnf a, f, u, v;
	halnf t;

	bool Trig::rayHit(class Ray& ray, vec3f& HitPos) {

		edge1 = V1 - V0;
		edge2 = V2 - V0;

		h = ray.Dir.cross(edge2);
		a = edge1.dot(h);

		if (a > -MATH_EPSILON && a < MATH_EPSILON) {
			return false;
		}

		f = 1.f / a;
		s = ray.Pos - V0;
		u = f * s.dot(h);

		if (u < 0.0 || u > 1.0) {
			return false;
		}

		q = s.cross(edge1);
		v = f * ray.Dir.dot(q);

		if (v < 0.f || u + v > 1.f) {
			return false;
		}

		t = f * edge2.dot(q);
		if (t > MATH_EPSILON) {
			HitPos = ray.Pos + ray.Dir * t;
			return true;
		} else {
			return false;
		}
	}

	alni Trig::sizeAllocatedMem() {
		return V0.sizeAllocatedMem() + V1.sizeAllocatedMem() + V2.sizeAllocatedMem();
	}

	alni Trig::sizeUsedMem() {
		return V0.sizeUsedMem() + V1.sizeUsedMem() + V2.sizeUsedMem();
	}

};