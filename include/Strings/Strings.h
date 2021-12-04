#pragma once

#include "Macros.h"

struct string;

#define STRR const string&

typedef alni str_idx;

int find_char(const char* c_str, int start, char val);
int cstr_len(const char* c_str);
str_idx cstrlen(const char* str);
str_idx cstrlen(const wchar_t* str);

void to_string(string* str, int val);
void to_string(string* str, alnf val);
void to_string(string* str, bool val);
void to_string(string* str, char val);

template <typename Type>
inline void to_string(string* str, Type val) {
  to_string(str, val);
}

bool str_from_string(string* str, int& val);
bool str_from_string(string* str, float& val);
bool str_from_string(string* str, bool& val);

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
  struct string *str;
  Range rng;
};

struct string {

  string(const char* str);
  string();

  template <typename Type>
  string(Type val) {
    to_string(this, val);
  }

  string(const string& val) {
    *this = val;
  }

  char* str = nullptr;

  str_idx len() const;
  void alloc(str_idx len);
  void clear();

  void operator=(const string& string);
  void operator=(const char* string);
  void operator=(const wchar_t* string);

  string& operator+=(const string& string);
  string operator+(const string& string);

  bool operator==(const char* cstring);
  bool operator==(const string& string);
  bool operator!=(const char* cstring);
  bool operator!=(const string& string);

  char operator[](str_idx idx);

  bool match(Range& range, string& str2, Range& range2);
  
  void copy(string* str, Range range);

  str_idx find(string& string, Range range) const;
  str_idx find(const char character, Range range) const;
  str_idx rfind(const char character, Range range) const;

  void trim(Range range);

  void assign(char* cs) {
    str = cs;
  }

  template <typename Type>
  bool to_val(Type& val) {
    return str_from_string(this, val);
  }

  int how_many(const char val);

  ~string();
};

#ifdef _DEBUG
#define CLOG(text) cnsl(text)
#else
#define CLOG(text) 
#endif 

void cnsl(const string& str);

void get_executable_dir(string* in);

const wchar_t* wide_char(const char* c);
