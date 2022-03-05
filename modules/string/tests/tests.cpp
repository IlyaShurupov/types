

#include "common.h"

#include "allocators.h"

#include "strings.h"

int main() {
	string str1;
	string& str2 = *new string();
	string& str3 = *new string();
	string str4 = "sadas";
	string str5(str4);
	string str6(str3);

	string str6(str4 + str1);
}