
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
	Node<Str*>* cursor = nullptr;
	List<Str*> buff;
public:

	void write(const struct Str& in);
	const Str* read();
};

void NdLog_write(const struct Str& in);

const Str* NdLog_read();