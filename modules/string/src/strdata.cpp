
#include "strdata.h"

#include "allocators.h"

str_data::str_data(str_user* p_owner, const char* p_buff, bool p_ref) {
	owner = p_owner;

	if (p_buff) {
		buff = (char*)p_buff;
	}
	else {
		buff = (char*)" ";
		p_ref = true;
	}

	refc = 0;

	flags.set(SD_CONST, p_ref);
	flags.set(SD_PROTECTED, true);
}

str_data::str_data(const str_data& in) {
	alni len = slen(in.buff);
	reserve(len);
	memcp(buff, in.buff, len);
}

str_data::~str_data() {
	if (!flags.get(SD_CONST)) {
		delete[] buff;
	}
}

void* str_data::operator new(size_t size) {
	static poolalloc alloc(sizeof(str_data), 100);
	return alloc.alloc(0);
}

void str_data::release() {
	if (buff) {
		delete[] buff;
	}
}

void str_data::reserve(alni len) {
	release();
	buff = new char[len + 1];
	buff[len] = '\0';
}

alni str_data::slen(const char* in) {
	alni out = 0;
	for (const char* iter = in; *iter != '\0'; iter++) {
		out++;
	}
	return out;
}

void str_data::insert(const char* in, alni atidx, alni len) {

}

void str_data::remove(alni start, alni end) {

}
