
#include "trigonometry.h"

namespace STD_MATH {
#include <math.h>
};

namespace trigs {
alnf sin(alnf radians) { return STD_MATH::sinf((halnf)radians); }
alnf tan(alnf radians) { return STD_MATH::tanf((halnf)radians); }
alnf cos(alnf radians) { return STD_MATH::cosf((halnf)radians); }
alnf sqrt(alnf val) { return STD_MATH::sqrt((halnf)val); }
alnf rad(alnf val) { return val * (PI / 180.f); }
alnf deg(alnf val) { return val * (180.f / PI); }
alnf atan2(alnf X, alnf Y) { return STD_MATH::atan2((halnf)X, (halnf)Y); }
alnf atan(alnf val) { return STD_MATH::atan((halnf)val); }
};  // namespace trigs