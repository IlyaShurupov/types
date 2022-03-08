
#include "common.h"

alni slen(const char* in);
char* sinsert(const char* cur, const char* tar, alni atidx, alni len);
void soverride(char* cur, const char* tar, alni atidx, alni len);
char* sremove(const char* cur, alni start, alni end);
alni sequal(const char* left, const char* right);

char* val2srt(int val, alni base);
char* val2srt(alni val, char* ownbuff, alni base);
char* val2srt(alnf val, alni base);
char* val2srt(bool val);
char* val2srt(char val);

bool str2val(const char* in, alni& val, alni base, alni ownlen);
bool str2val(const char* in, alnf& val, alni base);
bool str2val(const char* in, bool& val);

alni show_many(const char* str, const char val);
alni sfind(const char target, alni target_len, alni start, alni end, bool reversed);
