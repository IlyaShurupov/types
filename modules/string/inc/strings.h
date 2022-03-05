

#include "common.h"

class string {

	union str {
	  class heap_string* heapstr;
		const char* constr;
	} data;

	bits<alni> flags;
	void create_own_string();
	void refinc();
	void refdec();
	void prepare_modify();

public:

	string();
	string(const char* const_str);
	string(const string& class_str);

	const char* cstr();

	string& operator+=(string& in);

	~string();

	void* operator new(size_t size);
};