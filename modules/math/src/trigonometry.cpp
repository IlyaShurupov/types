
#include "trigonometry.h"

namespace STD_MATH {
	#include <math.h>
};


alnf sin(alnf radians) { return STD_MATH::sinf((float)radians); }
alnf cos(alnf radians) { return STD_MATH::cosf((float)radians); }
alnf sqrt(alnf val) { return STD_MATH::sqrt((float)val); }