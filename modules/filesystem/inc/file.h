
#pragma once

#include "strings.h"

// BAD! remove headers and refactor class
#include <fstream>

namespace tp {

	typedef std::fstream osfstream;

	enum osfile_openflags {
		SAVE,
		LOAD,
	};

	class File {
		osfstream fh;

		public:
		alni adress = 0;
		bool opened = false;

		alni avl_adress = 0;

		File() {}

		File(const string& path, osfile_openflags flags) { open(path, flags); }

		void open(const string& path, osfile_openflags);

		void write_bytes(const int1* in, alni size, alni adress = -1);

		template <typename Type>
		void write(Type* in, alni adress = -1) {
			write_bytes((int1*) in, sizeof(Type), adress);
		}

		void read_bytes(int1* in, alni size, alni adress = -1);

		template <typename Type>
		void read(Type* in, alni adress = -1) {
			read_bytes((int1*) in, sizeof(Type), adress);
		}

		void read(string& out);
		void write(const string& in);

		alni size();

		void fill(uint1 val, alni len);

		void close();

		~File() { close(); }

		alni sizeAllocatedMem() {
			alni out = 0;
			out += sizeof(osfstream);
			out += sizeof(alni) * 2;
			out += sizeof(bool) * 2;
			return 0;
		}
		alni sizeUsedMem() {
			return sizeAllocatedMem();
		}
	};

	string read_file(string path);

};