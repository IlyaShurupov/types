
#include "strdata.h"

#include "allocators.h"

#include "algorithms.h"

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
	buff = in.buff; 
	flags.set(SD_CONST, true);
	flags.set(SD_PROTECTED, true);
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
	if (buff && !flags.get(SD_CONST)) {
		delete[] buff;
	}
}

void str_data::reserve(alni len) {
	release();
	buff = new char[len + 1];
	buff[len] = '\0';
}

void str_data::clear() {
	if (buff && !flags.get(SD_CONST)) {
		delete[] buff;
	}
	flags.set(SD_CONST, true);
	buff = (char*)" ";
}

void str_data::insert(const char* in, alni atidx, alni len) {
	char* cur = buff;
	buff = sinsert(buff, in, atidx, len);
	delete[] cur;
}

void str_data::remove(alni start, alni end) {
	char* cur = buff;
	buff = sremove(buff, start, end);
	delete[] cur;
}

void str_data::override(const char* in, alni atidx, alni len) {
	soverride(buff, in, atidx, len);
}