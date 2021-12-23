
#pragma once

#include "Strings/Strings.h"

// BAD! remove headers and refactor class
#include <fstream>
typedef std::fstream OsFile;

enum FileOpenFlags {
	SAVE,
	LOAD,
};

class File {

	OsFile file;

public:

	alni adress = 0;
	bool opened = false;
	
	alni avl_adress = 0;

	File() {
	}

	File(const string& path, FileOpenFlags flags) {
		open(path, flags);
	}

	void open(const string& path, FileOpenFlags);

	void write_bytes(const int1* in, alni size, alni adress = -1);

	template < typename Type >
	void write(Type* in, alni adress = -1) {
		write_bytes((int1*)in, sizeof(Type), adress);
	}

	void read_bytes(int1* in, alni size, alni adress = -1);

	template < typename Type >
	void read(Type* in, alni adress = -1) {
		read_bytes((int1*)in, sizeof(Type), adress);
	}

	alni size();

	void fill(uint1 val, alni len);

	void close();

	~File() {
		close();
	}
};

