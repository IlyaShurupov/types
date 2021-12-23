
#pragma once

#ifdef _DEBUG
#define ND_LOG_dbg(val) NdLog_write(val)
#else
#define ND_LOG_dbg(val)
#endif // DEBUG

#define ND_LOG(val) NdLog_write(val)

#include "Containers/List.h"
#include "Strings/Strings.h"

class Nd_Log {
	Node<string*>* cursor = nullptr;
	List<string*> buff;
public:

	void write(const class string& in);
	const string* read();
};

void NdLog_write(const class string& in);

const string* NdLog_read();