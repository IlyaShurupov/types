
#pragma once

class typesExeption {

	const char* description;
	bool fatal;

public:
	typesExeption(const char* p_description, bool p_fatal);
};