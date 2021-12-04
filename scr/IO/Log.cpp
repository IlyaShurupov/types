
#include "IO/Log.h"

Nd_Log GLB_bd_log;

void Nd_Log::write(const struct Str& in) {
	buff.PushBack(new Str(in));

	if (!cursor) {
		cursor = buff.Last();
	}
}

const Str* Nd_Log::read() {
	if (cursor) {
		const Str* out = cursor->data;
		cursor = cursor->next;
		return out;
	}
	return nullptr;
}

void NdLog_write(const struct Str& in) {
	GLB_bd_log.write(in);
}

const Str* NdLog_read() {
	return GLB_bd_log.read();
}