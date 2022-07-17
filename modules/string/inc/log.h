
#pragma once

#include "list.h"
#include "strings.h"

namespace tp {

	class Logger {
		ListNode<string>* cursor = nullptr;
		List<string> buff;
		public:
		void write(string& in, bool post = false);
		string read();
	};

	extern Logger GLog;

	#ifdef _DEBUG
	#define DBGLOG(val) GLog.write((val), 1)
	#else
	#define DBGLOG(val) (0)
	#endif // DEBUG

};