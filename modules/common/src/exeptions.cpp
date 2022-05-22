
#include "exeptions.h"

namespace tp {
	Exeption::Exeption(const char* pDescription, bool pFatal) {
		mDescription = pDescription;
		mFatal = pFatal;
	}
};