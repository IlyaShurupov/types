

#include "common.h"

enum strdata_flags {
	SD_CONST,
	SD_PROTECTED,
};

class str_data {

	bits<uint4> flags; // strdata_flags enum flags
	uint4 refc; // number of users of this string data
	class str_user* owner; // only used to check if user is the owner (user == owner)
	char* buff; // actual string data
	
	friend str_user;

	str_data(str_user* p_owner, const char* p_buff, bool p_ref);
	str_data(const str_data& in);
	~str_data();

	void* operator new(size_t size);

	// control utils
	void release();
	void reserve(alni len);
	void assert_modifiable();

	// modification functionallity
	void clear();
	void insert(const char* in, alni atidx, alni len);
	void remove(alni start, alni end);
	void override(const char* in, alni atidx, alni len);
};