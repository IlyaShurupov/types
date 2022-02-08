

#include "random.h"

#include <iostream>


flt8 randf() {
	flt8 r = static_cast<flt8>(std::rand()) / static_cast<flt8>(RAND_MAX);
	return r;
}
