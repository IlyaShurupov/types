#pragma once

#include "common.h"
#include "algorithms.h"
#include "strdata.h"

namespace tp {

	class str_user {

		class str_data* datap;

		public:

		str_user();
		str_user(const char* in);
		str_user(char* str);
		str_user(const str_user& in);

		template <typename Type>
		str_user(Type val) {
			datap = new str_data(val2str(val), false);
			refinc(datap);
		}

		explicit operator alni() const;
		explicit operator alnf() const;
		explicit operator bool() const;
		//operator const char*() const;

		~str_user();

		private: // own utils
		void refinc(str_data* dp);
		void refdec(str_data* dp);
		void assert_modifiable();

		public: // data info access 
		const char* cstr() const;
		char operator[](alni idx) const;
		alni size() const;

		bool operator==(const char* cstring) const;
		bool operator==(const str_user& string) const;
		bool operator!=(const char* cstring) const;
		bool operator!=(const str_user& string) const;

		public: // editing interface	

		str_user& capture();

		char* get_writable();
		void reserve(alni len);

		str_user& insert(const str_user& string, alni at, alni len = 0);
		str_user& insert(const char* string, alni at, alni len = 0);

		str_user& remove(Range range);

		void operator=(const str_user& string);
		void operator=(const char* string);

		str_user& operator+=(const str_user& string);
		str_user& operator+=(const char* string);
		str_user operator+(const str_user& string) const;
		str_user operator+(const char* string) const;

		void trim(Range range);
		void clear();

		alni save_size() const ;
		void save(class File* file) const;
		void load(class File* file);
	};

	str_user sfmt(const char* fmt, ...);

	alni hash(const str_user&);
};