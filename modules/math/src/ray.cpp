
#include "ray.h"

Ray::Ray() {
}

Ray::Ray(const vec3f& Dir, const vec3f& Pos) {
	this->Dir = Dir;
	this->Pos = Pos;
}

Ray::~Ray() {
}
