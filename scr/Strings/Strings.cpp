
#include "Strings/Strings.h"

#include <cstdio> // scanf

#include "Macros.h"

str_idx cstrlen(const char* str) {
  str_idx len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

str_idx cstrlen(const wchar_t* str) {
  str_idx len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

str_idx Range::len() {
  return end - strt + 1;
}

bool Range::valid() {
  return (strt > 0 && end > 0 && end >= strt);
}

Range::Range() {
  strt = end = -1;
}

Range::Range(str_idx strt, str_idx end) {
  this->strt = strt;
  this->end = end;
}

Range::Range(const Range& rng) {
  strt = rng.strt;
  end = rng.end;
}


Str::Str() {}

Str::~Str() {
  clear();
}

Str::Str(const char* string) {
  alloc(cstrlen(string));
  for (str_idx i = 0; i < length; i++) {
    str[i] = string[i];
  }
}

str_idx Str::len() {
  return length;
}

void Str::alloc(str_idx len) {
  clear();

  str = new char[len + 1];

  str[len] = '\0';
  length = len;
}

void Str::clear() {
  if (str) {
    delete [] str;
  }
}

void Str::operator=(const Str& string) {

  alloc(string.length);

  for (str_idx i = 0; i < string.length; i++) {
    str[i] = string.str[i];
  }

  length = string.length;
}

void Str::operator=(const wchar_t* string) {

  alloc(cstrlen(string));

  for (str_idx i = 0; i < cstrlen(string); i++) {
    str[i] = string[i];
  }

  length = cstrlen(string);
}

Str& Str::operator+=(const Str& string) {

  str_idx newlen = string.length + length;
  
  if (!newlen) {
    return *this;
  }

  char* newstr = new char [newlen + 1];
  newstr[newlen] = '\0';

  for (str_idx i = 0; i < length; i++) {
    newstr[i] = str[i];
  }

  for (str_idx i = 0; i < string.length; i++) {
    newstr[i + length] = string.str[i];
  }

  clear();

  str = newstr;
  length = newlen;
  return *this;
}

Str Str::operator+(const Str& string) {
  return Str(*this) += string;
}

void Str::operator=(const char* string) {
  alloc(cstrlen(string));
  for (str_idx i = 0; i < length; i++) {
    str[i] = string[i];
  }
}

bool Str::operator==(const char* cstring) {
  if (cstrlen(cstring) != length) {
    return false;
  }
  for (long i = 0; i < length; i++) {
    if (str[i] != cstring[i]) {
      return false;
    }
  }
  return true;
}

bool Str::operator==(const Str& string) {
  if (string.length != length) {
    return false;
  }
  for (long i = 0; i < length; i++) {
    if (str[i] != string.str[i]) {
      return false;
    }
  }
  return true;
}

bool Str::operator!=(const Str& string) {
  return !(operator==(string));
}

bool Str::operator!=(const char* cstring) {
  return !(operator==(cstring));
}

char Str::operator[](str_idx idx) {
  return str[idx];
}

bool Str::match(Range& range, Str& str2, Range& range2) {
  if (range.len() != range2.len()) {
    return false;
  }
  for (str_idx i = 0; i < range.len(); i++) {
    if (str[range.strt + i] != str2[range2.strt + i]) {
      return false;
    }
  }
  return true;
}

void Str::coppy(Str* str, Range range) {
  alloc(range.len());
  for (str_idx i = 0; i < length; i++) {
    this->str[i] = (*str)[i + range.strt];
  }
}

str_idx Str::find(Str& string, Range range) const {
  for (str_idx i = range.strt; i < range.end; i++) {

    str_idx m = 0;
    while (m < string.len()) {
      if (this->str[i] != str[m]) {
        break;
      }
      m++;
    }

    if (m == string.len() - 1) {
      return i;
    }
  }
  return -1;
}

str_idx Str::find(const char character, Range range) const {
  for (str_idx i = range.strt; i <= range.end; i++) {
    if (character == str[i]) {
      return i;
    }
  }
  return -1;
}

str_idx Str::rfind(const char character, Range range) const {
  for (str_idx i = range.end; i >= range.strt; i--) {
    if (character == str[i]) {
      return i;
    }
  }
  return -1;
}

void Str::trim(Range range) {
  str_idx newlen = range.len();
  char* newstr = new char [newlen + 1];
  newstr[newlen] = '\0';

  for (str_idx i = 0; i < newlen; i++) {
    newstr[i] = str[i + range.strt];
  }

  clear();
  str = newstr;
  length = newlen;
}

int Str::how_many(const char val) {
  int out = 0;
  for (str_idx i = 0; i < length; i++) {
    if (str[i] == val) {
      out++;
    }
  }
  return out;
}

void to_string(Str* str, int val) {
  to_string(str, (int8)val);
}

void to_string(Str* str, int8 val) {
  alni iter = val;
  alni len = 0;
  while (iter /= 10) {
    len++;
  }

  bool neg = val < 0;

  len += 1 + int(neg);

  str->alloc(len);

  val = ABS(val);

  for (alni i = len - 1; i >= int(neg); i--) {
    str->str[i] = (char)(val % 10 + 48);
    val /= 10;
  }

  if (neg) {
    str->str[0] = '-';
  }
}

void to_string(Str* str, alnf val) {
  alni left_side = (alni)val;
  alni mantissa = (val - left_side) * 100000;

  alni len = 3;

  while (left_side /= 10) {
    len++;
  }

  while (mantissa /= 10) {
    len++;
  }

  str->alloc(len);
  alni ret = snprintf(str->str, len * sizeof(char) + 1, "%f", val);
}

void to_string(Str* str, bool val) {
  if (val) {
    *str = "True";
    return;
  }

  *str = "False";
}

void to_string(Str* str, char val) {
  str->alloc(1);
  str->str[0] = val;
}

bool str_from_string(Str* str, int& val) {
  return sscanf(str->str, "%d", &val) == 1;
}

bool str_from_string(Str* str, float& val) {
  return sscanf(str->str, "%f", &val) == 1;
}

bool str_from_string(Str* str, bool& val) {
  if (str->length == 0) {
    return false;
  }

  if (*str == "False" || *str == "false" || *str == "0") {
    val = 0;
    return true;
  }
  val = 1;

  return true;
}


void cnsl(const Str& str) {
  printf(str.str);
}


int find_char(const char* c_str, int start, char val) {
  for (int idx = start; c_str[idx] != '\0'; idx++) {
    if (val == c_str[idx]) {
      return idx;
    }
  }
  return -1;
}

int cstr_len(const char* c_str) {
  int idx = 0;
  for (; c_str[idx] != '\0'; idx++) {
  }
  return idx;
}

#ifdef _WIN

#include <Windows.h>

void get_executable_dir(Str* in) {
  TCHAR szFileName[MAX_PATH];
  GetModuleFileName(NULL, szFileName, MAX_PATH);

  *in = szFileName;
  in->trim(Range(0, in->rfind('\\', Range(0, in->length))));
}

const wchar_t* wide_char(const char* c) {

  const size_t cSize = cstrlen(c) + 1;
  wchar_t* wc = new wchar_t[cSize];
  mbstowcs(wc, c, cSize);

  return wc;
}

#else

void get_executable_dir(Str* in) {
}

#endif // WIN


