
#include "IO/File.h"

#include <iostream>

#ifdef _WIN32
#include <Windows.h>

bool create_dir(const char* path) {

	uint4 path_len = cstr_len(path) + 1;

	wchar_t* wtext = new wchar_t[path_len];

	mbstowcs(wtext, path, path_len);

	LPWSTR ptr = wtext;

	bool res = CreateDirectory(ptr, NULL) || ERROR_ALREADY_EXISTS == GetLastError();
	delete[] wtext;

	return res;
}

#endif

void File::open(const Str& path) {

	if (!opened) {
		file.open(path.str, std::ios::in | std::ios::out | std::ios::binary);

		/*
		//get length of file
		file.seekg(0, std::ios::end);
		size_t length = file.tellg();
		file.seekg(0, std::ios::beg);
		size = (alni)length;
		*/

		opened = true;
	}
}

bool File::create(const Str& path) {

	alni dir_idx_1 = path.rfind('\\', Range(0, path.length));
	alni dir_idx_2 = path.rfind('/', Range(0, path.length));

	if (dir_idx_2 > dir_idx_1) {
		dir_idx_1 = dir_idx_2;
	}

	Str directory(path);

	directory.trim(Range(0, dir_idx_1 - 1));

	bool done = create_dir(directory.str);

	if (done) {
		file.open(path.str, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
	}

	opened = done && file.is_open();
	return opened;
}

void File::close() {
	if (opened) {
		file.close();
	}
}


void File::write(const uint1* in, int size, alni p_adress) {
	if (p_adress == -1) {
		p_adress = this->adress;
		file.seekp(p_adress);
		file.write((const char*)in, size);
		return;
	}
	file.seekp(p_adress);
	file.write((const char*)in, size);
}


void File::read(const uint1* in, int size, alni p_adress) {
	if (p_adress == -1) {
		p_adress = this->adress;
		//file.read(p_adress, in, size);
		return;
	}
	//file.read(this->adress, in, size);
}

void File::fill(uint1 val, int len) {
	for (int i = 0; i < len; i++) {
		write<uint1>(&val, i);
	}
}