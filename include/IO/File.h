
#pragma once

#include "Strings/Strings.h"

// BAD! remove headers and refactor class
#include <fstream>
typedef std::ofstream OsFile;

class File {

	OsFile file;

public:

	alni adress = 0;
	bool opened = false;
	char* buff = nullptr;
	alni size = 0;

	File() {
	}

	File(const Str& path) {
		open(path);
	}

	void open(const Str& path);

	bool create(const Str& path);

	void write(const uint1* in, int size, alni adress = -1);

	template < typename Type >
	void write(Type* in, alni adress = -1) {
		write((uint1*)in, sizeof(Type), adress);
	}


	void read(const uint1* in, int size, alni adress = -1);

	template < typename Type >
	void read(Type* in, alni adress = -1) {
		read((uint1*)in, sizeof(Type), adress);
	}


	void fill(uint1 val, int len);

	void close();

	~File() {
		close();
	}
};

