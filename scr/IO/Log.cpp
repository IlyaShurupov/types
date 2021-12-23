
#include "IO/Log.h"

Nd_Log GLB_bd_log;

void Nd_Log::write(const class string& in) {
	buff.PushBack(new string(in));

	if (!cursor) {
		cursor = buff.Last();
	}
}

const string* Nd_Log::read() {
	if (cursor) {
		const string* out = cursor->data;
		cursor = cursor->next;
		return out;
	}
	return nullptr;
}

void NdLog_write(const class string& in) {
	GLB_bd_log.write(in);
}

const string* NdLog_read() {
	return GLB_bd_log.read();
}