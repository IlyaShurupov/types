
#include "strdata.h"

#include "allocators.h"

#include "algorithms.h"

namespace tp {

	str_data::str_data(const char* p_buff, bool p_ref) {

		if (p_buff) {
			buff = (char*) p_buff;
		} else {
			buff = (char*) " ";
			p_ref = true;
		}

		refc = 0;

		is_const = p_ref;
	}

	str_data::str_data(const str_data& in) {
		buff = in.buff;
		is_const = true;
		refc = 0;
	}

	str_data::~str_data() {
		if (!is_const) {
			delete[] buff;
		}
	}

	void* str_data::operator new(size_t size) {
		static PoolAlloc alloc(sizeof(str_data), 100);
		return alloc.Alloc(0);
	}

	void str_data::release() {
		if (buff && !is_const) {
			delete[] buff;
		}
	}

	void str_data::reserve(alni len) {
		release();
		buff = new char[len + 1];
		buff[len] = '\0';
		is_const = false;
	}

	void str_data::clear() {
		if (buff && !is_const) {
			delete[] buff;
		}
		is_const = true;
		buff = (char*) " ";
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

};