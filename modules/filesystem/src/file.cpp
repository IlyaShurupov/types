
#include "file.h"

#include <iostream>

namespace tp {

	void File::open(const string& path, osfile_openflags flags) {
		adress = 0;
		avl_adress = 0;

		if (!opened) {
			switch (flags) {
				case SAVE:
					fh.open(path.cstr(),
						std::ios::out | std::ios::binary | std::ios::trunc);
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

	void File::close() {
		if (opened) {
			fh.close();
			opened = false;
		}
	}

	void File::write_bytes(const int1* in, alni size, alni p_adress) {
		if (p_adress == -1) {
			p_adress = this->adress;
			this->adress += size;
		}

		fh.seekp(p_adress);
		fh.write(in, size);
	}

	void File::read_bytes(int1* in, alni size, alni p_adress) {
		if (p_adress == -1) {
			p_adress = this->adress;
			adress += size;
		}
		fh.seekg(p_adress);
		fh.read(in, size);
	}

	void File::fill(uint1 val, alni len) {
		for (int i = 0; i < len; i++) {
			write<uint1>(&val, i);
		}
	}

	alni File::size() {
		alni out = 0;
		fh.seekg(0, std::ios::beg);
		out = fh.tellg();
		fh.seekg(0, std::ios::end);
		out = fh.tellg() - out;
		return out;
	}

	void File::read(string& out) {
		alni tmp_adress = adress;
		int1 val = -1;
		alni len = 0;
		while (val != '\0') {
			read_bytes(&val, 1);
			len++;
		}
		adress = tmp_adress;
		assert(len);

		out.reserve(len);

		read_bytes((int1*) out.get_writable(), sizeof(int1) * len);
	}

	void File::write(const string& in) {
		write_bytes((int1*) in.cstr(), sizeof(int1) * in.size() + 1);
	}

	string read_file(string path) {
		File file;
		file.open(path, osfile_openflags::LOAD);

		alni len = file.size();
		string script;
		script.reserve(len);

		int1* buff = script.get_writable();
		file.read_bytes(buff, len, 0);
		return script;
	}

};