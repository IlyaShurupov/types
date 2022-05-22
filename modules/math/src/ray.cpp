
#include "ray.h"

namespace tp {

	Ray::Ray() {}

	Ray::Ray(const vec3f& Dir, const vec3f& Pos) {
		this->Dir = Dir.unitv();
		this->Pos = Pos;
	}

	Ray::~Ray() {}

};