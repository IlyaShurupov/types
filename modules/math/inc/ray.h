#pragma once

#include "vec.h"

namespace tp {

	class Ray {
		public:
		vec3f Dir;
		vec3f Pos;

		public:
		Ray(const vec3f& Dir, const  vec3f& Pos);
		Ray();
		~Ray();

		alni sizeAllocatedMem();
		alni sizeUsedMem();
	};

};