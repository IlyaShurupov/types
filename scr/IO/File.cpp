
#include "IO/File.h"

#include <iostream>


void File::open(const string& path, FileOpenFlags flags) {

	if (!opened) {
		
		switch (flags) {
			case SAVE:
				file.open(path.str, std::ios::out | std::ios::binary | std::ios::trunc);
				break;

			case LOAD:
				file.open(path.str, std::ios::in | std::ios::binary | std::ios::app);
				break;
		};

		if (!file.is_open()) {

			assert(file.is_open());
		}
		opened = true;
	}
}

void File::close() {
	if (opened) {
		file.close();
	}
}


void File::write_bytes(const int1* in, alni size, alni p_adress) {
	if (p_adress == -1) {
		p_adress = this->adress;
		this->adress += size;
	}

	file.seekp(p_adress);
	file.write(in, size);
}


void File::read_bytes(int1* in, alni size, alni p_adress) {
	if (p_adress == -1) {
		p_adress = this->adress;
		adress += size;
	}
	file.seekg(p_adress);
	file.read(in, size);
}

void File::fill(uint1 val, alni len) {
	for (int i = 0; i < len; i++) {
		write<uint1>(&val, i);
	}
}


alni File::size() {
	alni out = 0;
	file.seekg(0, std::ios::beg);
	out = file.tellg();
	file.seekg(0, std::ios::end);
	out = file.tellg() - out;
	return out;
}