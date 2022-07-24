
#include "log.h"

#include <stdio.h>

namespace tp {

	Logger Glog;

	void Logger::write(string& in, bool post) {
		buff.pushBack(new string(in));

		if (!cursor) {
			cursor = buff.last();
		}

		if (post) {
			printf(in.cstr());
		}
	}

	string Logger::read() {
		if (cursor) {
			const string out = cursor->data;
			cursor = cursor->next;
			return out;
		}
		return string();
	}

	alni Logger::sizeAllocatedMem() {
		alni out = sizeof(ListNode<string>*); // cursor
		out += buff.sizeAllocatedMem();
		return out;
	}

	alni Logger::sizeUsedMem() {
		alni out = sizeof(ListNode<string>*); // cursor
		out += buff.sizeUsedMem();
		return out;
	}

};