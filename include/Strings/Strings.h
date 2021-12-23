#pragma once

#include "Macros.h"


class string;

int find_char(const char* c_str, int start, char val);
int cstr_len(const char* c_str);
alni cstrlen(const char* str);
alni cstrlen(const wchar_t* str);

void to_string(string* str, int val);
void to_string(string* str, alni val);
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
  alni strt;
  alni end;

  alni len();
  bool valid();
  Range();
  Range(const Range& rng);
  Range(alni strt, alni end);
};

struct StrRef {
  string *str;
  Range rng;
};

class string {

public:

  char* str = nullptr;
  alni flags = -1;

  string();
  string(const char* str);
  string(char* str);

  template <typename Type>
  string(Type val) {
    to_string(this, val);
  }

  string(const string& val) {
    *this = val;
  }


  alni len() const;
  void alloc(alni len);
  void clear();

  void operator=(const string& string);
  void operator=(const char* string);
  void operator=(const wchar_t* string);

  string& operator+=(const string& string);
  string operator+(const string& string);

  bool operator==(const char* cstring) const;
  bool operator==(const string& string) const;
  bool operator!=(const char* cstring) const;
  bool operator!=(const string& string) const;

  char operator[](alni idx) const;

  bool match(Range& range, string& str2, Range& range2);
  
  void copy(string* str, Range range);

  alni find(string& string, Range range) const;
  alni find(const char character, Range range) const;
  alni rfind(const char character, Range range) const;

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
