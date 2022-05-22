
#include "common.h"

#include <stdlib.h>

namespace tp {

	void memsetv(void* p, alni bytesize, uint1 val) {

		alni alignedval = 0;
		for (alni idx = 0; idx < sizeof(alni); idx++) {
			((uint1*) &alignedval)[idx] = val;
		}

		alni alignedlen = bytesize / sizeof(alni);
		for (alni idx = 0; idx < alignedlen; idx++) {
			((alni*) p)[idx] = val;
		}

		alni unalignedlen = bytesize - (alignedlen * sizeof(alni));
		for (alni idx = 0; idx < unalignedlen; idx++) {
			((uint1*) p)[bytesize - idx - 1] = val;
		}
	}

	void memcp(void* left, const void* right, alni len) {
		alni alignedlen = len / sizeof(alni);
		for (alni idx = 0; idx < alignedlen; idx++) {
			((alni*) left)[idx] = ((alni*) right)[idx];
		}

		alni unalignedlen = len - (alignedlen * sizeof(alni));
		for (alni idx = 0; idx < unalignedlen; idx++) {
			((uint1*) left)[len - idx - 1] = ((uint1*) right)[len - idx - 1];
		}
	}

	bool memequal(const void* left, const void* right, alni len) {
		alni alignedlen = len / sizeof(alni);
		for (alni idx = 0; idx < alignedlen; idx++) {
			if (((alni*) left)[idx] != ((alni*) right)[idx]) {
				return false;
			}
		}

		alni unalignedlen = len - (alignedlen * sizeof(alni));
		for (alni idx = 0; idx < unalignedlen; idx++) {
			if (((uint1*) left)[len - idx - 1] != ((uint1*) right)[len - idx - 1]) {
				return false;
			}
		}
		return true;
	}

	uint8 next_pow_of_2(uint8 v) {
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v |= v >> 32;
		return v + 1;
	}

	alni hash(const char* bytes) {
		unsigned long hash = 5381;
		int c;
		while ((c = *bytes++)) {
			hash = ((hash << 5) + hash) + c;
		}
		return hash;
	}

	alni hash(alni bytes) {
		return bytes;
	}

	void char2wide(const char* c, wchar_t* out, alni len) {
		mbstowcs(out, c, len);
	}

	void wide2char(const wchar_t* c, char* out) {
		alni len = 0; for (const wchar_t* iter = c; *iter != '\0'; iter++) { len++; }
		wcstombs(out, c, len);
	}

	#ifdef PLATFORM_WINDOWS

	#include <Windows.h>

	const char* working_dir() {
		static TCHAR szFileName[MAX_PATH];
		static char out[MAX_PATH];
		static bool initialized = false;
		if (!initialized) {
			GetModuleFileName(NULL, szFileName, MAX_PATH);
			wide2char(szFileName, out);
			for (char* iter = out + strlen(out); ; iter--) {
				if (iter == "\\") {
					*(iter++) = '\0';
					break;
				}
			}
		}
		return out;
	}

	#endif // WIN

};