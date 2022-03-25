#pragma once

#include "common.h"

enum strdata_flags {
  SD_CONST,
};

class str_data {
  bits<uint4> flags;  // strdata_flags enum flags
  uint4 refc;         // number of users of this string data
  char* buff;         // actual string data

  friend class str_user;

  str_data(const char* p_buff, bool p_ref);
  str_data(const str_data& in);
  ~str_data();

  void* operator new(size_t size);

  // control utils
  void release();
  void reserve(alni len);

  // modification functionallity
  void clear();
  void insert(const char* in, alni atidx, alni len);
  void remove(alni start, alni end);
  void override(const char* in, alni atidx, alni len);
};