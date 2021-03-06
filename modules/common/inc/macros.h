
#pragma once

#define EPSILON 0.0000001

#define PI double(3.1415926535897932384626433832795)
#define PI2 (PI * 2)
#define PI13 (PI / 3)
#define PI23 (PI2 / 3)
#define PI43 (2 * PI2 / 3)

#define SQRT2 1.4142135623730950488016887242
#define Exp 2.7182818284590452353602874714

#define ABS(val) (val) > 0 ? (val) : (-val)
#define CLAMP(val, lower_lim, upper_lim) if (val > upper_lim) { val = upper_lim; } else if (val < lower_lim) {val = lower_lim;}
#define INRANGE(val, bd1, bd2) ((val) >= (bd1) && (val) <= (bd2))
#define SWAP(val1, val2, type) type tmp = (val1); (val1) = (val2); (val2) = tmp
#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

#define PTR_OFFSET(first, offset) (*((&first) + offset))
#define MEMBER_OFFSET(s, m) (alni(&(((s*)0)->m)))


#define assert_static(x, message) static_assert(x, message)