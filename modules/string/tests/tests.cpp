

#include "common.h"

#include "allocators.h"

#include "strings.h"

#include <iostream> 

void f(string g) {
}

int main() {
	string str1;
	string& str2 = *new string();
	string& str3 = *new string();
	string str4 = "str";
	string str5(str4);
	string str6(str3);

	str5 += "add";
	
	std::cout << str5.cstr();

	f("sad");
}