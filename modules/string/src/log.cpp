
#include "log.h"

#include <stdio.h>

Logger Glog;

void Logger::write(string& in, bool post) {
	buff.PushBack(new string(in));

	if (!cursor) {
		cursor = buff.Last();
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