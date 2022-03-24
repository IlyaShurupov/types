
#include "file.h"

#include <iostream>


void osfile::open(const string& path, osfile_openflags flags) {
	adress = 0;
	avl_adress = 0;

	if (!opened) {

		switch (flags) {
		case SAVE:
			fh.open(path.cstr(), std::ios::out | std::ios::binary | std::ios::trunc);
			break;

		case LOAD:
			fh.open(path.cstr(), std::ios::in | std::ios::binary | std::ios::app);
			break;
		};

		if (!fh.is_open()) {
			opened = false;
			return;
		}
		opened = true;
	}
}

void osfile::close() {
	if (opened) {
		fh.close();
		opened = false;
	}
}


void osfile::write_bytes(const int1* in, alni size, alni p_adress) {
	if (p_adress == -1) {
		p_adress = this->adress;
		this->adress += size;
	}

	fh.seekp(p_adress);
	fh.write(in, size);
}


void osfile::read_bytes(int1* in, alni size, alni p_adress) {
	if (p_adress == -1) {
		p_adress = this->adress;
		adress += size;
	}
	fh.seekg(p_adress);
	fh.read(in, size);
}

void osfile::fill(uint1 val, alni len) {
	for (int i = 0; i < len; i++) {
		write<uint1>(&val, i);
	}
}


alni osfile::size() {
	alni out = 0;
	fh.seekg(0, std::ios::beg);
	out = fh.tellg();
	fh.seekg(0, std::ios::end);
	out = fh.tellg() - out;
	return out;
}


string read_file(string path) {
	osfile file;
	file.open(path, osfile_openflags::LOAD);

	string script;
	script.reserve(file.size());

	file.read_bytes(script.get_writable(), file.size(), 0);
	return script;
}