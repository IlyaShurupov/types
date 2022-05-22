
#pragma once

namespace tp {

	class Exeption {

		const char* mDescription;
		bool mFatal;

		public:
		Exeption(const char* pDescription, bool pFatal);
	};

};