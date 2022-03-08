#pragma once

#include "common.h"
#include "algorithms.h"
#include "strdata.h"

class str_user {

	class str_data* datap;

public:

	str_user();
	str_user(const char* in);
	str_user(char* str);
	str_user(const str_user& in);

	template <typename Type>
	str_user(Type val) {
		datap = new str_data(this, val2str(val), false);
		refinc(datap);
	}

	operator alni() const;
	operator alnf() const;
	operator bool() const;

	~str_user();

private: // own utils
	void refinc(str_data* dp);
	void refdec(str_data* dp);
	void assert_modifiable();

public: // policy interface	
	void capture();
	bool get_protected();
	void set_protected(bool val);

public: // data info access 
	const char* cstr() const;
	char operator[](alni idx) const;
	alni size() const;

	bool operator==(const char* cstring) const;
	bool operator==(const str_user& string) const;
	bool operator!=(const char* cstring) const;
	bool operator!=(const str_user& string) const;

public: // editing interface	

	char* get_writable();
	void reserve(alni len);

	str_user& insert(const str_user& string, alni at, alni len = 0);
	str_user& insert(const char* string, alni at, alni len = 0);

	str_user& remove(range range);

	void operator=(const str_user& string);
	void operator=(const char* string);

	str_user& operator+=(const str_user& string);
	str_user& operator+=(const char* string);
	str_user operator+(const str_user& string);
	str_user operator+(const char* string);

	void trim(range range);
	void clear();
};