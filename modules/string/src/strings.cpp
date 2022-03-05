
#include "strings.h"

#include "common.h"
#include "allocators.h"

class heap_string {

	friend class string;

	alni refc = 0;
	char* buff;

	void release() {
		if (buff) {
			delete[] buff;
		}
	}

	void reserve(alni len) {
		release();
		buff = new char[len + 1];
		buff[len] = '\0';
	}

	alni slen(const char* in) {
		alni out = 0;
		for (const char* iter = in; *iter != '\0'; iter++) {
			out++;
		}
		return out;
	}



public:

	heap_string() {
		buff = NULL;
	}

	heap_string(heap_string* hs) {
		copy(hs->buff);
	}

	heap_string(const char* constr) {
		copy(constr);
	}

	void insert(const char* block, alni at, alni block_len) {
		//
	}

	void copy(const char* constr) {
		alni len = slen(constr);
		reserve(len);
		memcp(buff, constr, len);
	}

	void append(const char* block) {
		insert(block, -1, slen(block));
	}

	~heap_string() {
		release();
	}


	void* operator new(size_t size) {
		static poolalloc alloc(sizeof(heap_string), 100);
		return alloc.alloc(0);
	}
};


enum string_flags {
	STR_INITIALIZED,
	STR_CONST,
	STR_OWNER,
};

void string::create_own_string() {

	if (flags.get(STR_CONST)) {
		data.heapstr = new heap_string(data.constr);
	}
	else {
		heap_string* hs = new heap_string(data.heapstr);
		refdec();
		data.heapstr = hs;
	}

	flags.set(STR_INITIALIZED, true);
	flags.set(STR_CONST, false);
	flags.set(STR_OWNER, true);


	refinc();
}

void string::refinc() {
	if (!flags.get(STR_INITIALIZED) || flags.get(STR_CONST)) {
		return;
	}

	data.heapstr->refc++;
}

void string::refdec() {
	if (!flags.get(STR_INITIALIZED) || flags.get(STR_CONST)) {
		return;
	}

	data.heapstr->refc--;
	if (!data.heapstr->refc) {
		delete data.heapstr;

		flags.set(STR_INITIALIZED, false);
	}
}

void string::prepare_modify() {
	if (!flags.get(STR_INITIALIZED) || flags.get(STR_CONST) || !flags.get(STR_OWNER)) {
		create_own_string();
	}
}


string::string() {
	flags.set(STR_INITIALIZED, false);
}

string::string(const char* const_str) {
	flags.set(STR_INITIALIZED, true);
	flags.set(STR_CONST, true);

	data.constr = const_str;
}

string::string(const string& class_str) {
	flags = class_str.flags;

	if (!flags.get(STR_INITIALIZED)) {
		return;
	}

	if (flags.get(STR_CONST)) {
		data.constr = class_str.data.constr;
	}
	else {
		flags.set(STR_OWNER, false);
		data.heapstr = class_str.data.heapstr;
		refinc();
	}
}

const char* string::cstr() {
	if (!flags.get(STR_INITIALIZED)) {
		return NULL;
	}

	if (flags.get(STR_CONST)) {
		return data.constr;
	}
	else {
		return data.heapstr->buff;
	}
}

string& string::operator+=(string& in) {
	prepare_modify();

	return *this;
}

string::~string() {
	refdec();
}

void* string::operator new(size_t size) {
	static poolalloc alloc(sizeof(string), 100);
	return alloc.alloc(0);
}
