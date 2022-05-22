
#include "trigonometry.h"

namespace STD_MATH {
	#include <math.h>
};

namespace tp {
	alnf sin(const alnf radians) { return STD_MATH::sinf((halnf) radians); }
	alnf tan(const alnf radians) { return STD_MATH::tanf((halnf) radians); }
	alnf cos(const alnf radians) { return STD_MATH::cosf((halnf) radians); }
	alnf acos(const alnf val) { return STD_MATH::acos((halnf) val); }
	alnf sqrt(const alnf val) { return STD_MATH::sqrt((halnf) val); }
	alnf rad(const alnf val) { return val * (PI / 180.f); }
	alnf deg(const alnf val) { return val * (180.f / PI); }
	alnf atan2(const alnf X, const alnf Y) { return STD_MATH::atan2((halnf) X, (halnf) Y); }
	alnf atan(const alnf val) { return STD_MATH::atan((halnf) val); }
};  // namespace trigs