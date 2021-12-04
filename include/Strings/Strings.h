#pragma once

#include "Macros.h"

struct Str;

#define STRR const Str&

typedef alni str_idx;

int find_char(const char* c_str, int start, char val);
int cstr_len(const char* c_str);
str_idx cstrlen(const char* str);
str_idx cstrlen(const wchar_t* str);

void to_string(Str* str, int val);
void to_string(Str* str, alnf val);
void to_string(Str* str, bool val);
void to_string(Str* str, char val);

template <typename Type>
inline void to_string(Str* str, Type val) {
  to_string(str, val);
}

bool str_from_string(Str* str, int& val);
bool str_from_string(Str* str, float& val);
bool str_from_string(Str* str, bool& val);

struct Range {
  str_idx strt;
  str_idx end;

  str_idx len();
  bool valid();
  Range();
  Range(const Range& rng);
  Range(str_idx strt, str_idx end);
};

struct StrRef {
  struct Str *str;
  Range rng;
};

struct Str {

  Str(const char* str);
  Str();

  template <typename Type>
  Str(Type val) {
    to_string(this, val);
  }

  Str(const Str& val) {
    *this = val;
  }

  char* str = nullptr;
  str_idx length = 0;

  str_idx len();
  void alloc(str_idx len);
  void clear();

  void operator=(const Str& string);
  void operator=(const char* string);
  void operator=(const wchar_t* string);

  Str& operator+=(const Str& string);
  Str operator+(const Str& string);

  bool operator==(const char* cstring);
  bool operator==(const Str& string);
  bool operator!=(const char* cstring);
  bool operator!=(const Str& string);

  char operator[](str_idx idx);

  bool match(Range& range, Str& str2, Range& range2);
  
  void coppy(Str* str, Range range);

  str_idx find(Str& string, Range range) const;
  str_idx find(const char character, Range range) const;
  str_idx rfind(const char character, Range range) const;

  void trim(Range range);

  void assign(char* cs) {
    str = cs;
    length = cstrlen(cs);
  }

  template <typename Type>
  bool to_val(Type& val) {
    return str_from_string(this, val);
  }

  int how_many(const char val);

  ~Str();
};

#ifdef _DEBUG
#define CLOG(text) cnsl(text)
#else
#define CLOG(text) 
#endif 

void cnsl(const Str& str);

void get_executable_dir(Str* in);

const wchar_t* wide_char(const char* c);
