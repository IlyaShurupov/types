
#include <cstdarg>

#include "strdata.h"
#include "strings.h"

#include "filesystem.h"

str_user::str_user() {
  datap = new str_data(NULL, true);
  refinc(datap);
}

str_user::str_user(const char* in) { operator=(in); }

str_user::str_user(char* in) { operator=((const char*)in); }

str_user::str_user(const str_user& in) {
  datap = NULL;
  operator=(in);
}

str_user::operator alni() const {
  alni val;
  return (datap && datap->buff && str2val(datap->buff, val, 10, NULL)) ? val
                                                                       : -1;
}

str_user::operator alnf() const {
  alnf val;
  return (datap && datap->buff && str2val(datap->buff, val, 10)) ? val : -1;
}

str_user::operator bool() const {
  bool val;
  return (datap && datap->buff && str2val(datap->buff, val)) ? val : -1;
}

const char* str_user::cstr() const { return datap->buff; }

char str_user::operator[](alni idx) const {
  return (datap && datap->buff) ? datap->buff[idx] : 0;
}

alni str_user::size() const {
  return (datap && datap->buff) ? slen(datap->buff) : 0;
}

str_user::~str_user() { refdec(datap); }

void str_user::refinc(str_data* dp) { dp->refc++; }

void str_user::refdec(str_data* dp) {
  dp->refc--;
  if (!dp->refc) {
    delete datap;
  }
}

void str_user::assert_modifiable() {
  // if have no rights to modify create new copy of string data
  if (datap->refc > 1) {
    refdec(datap);
    datap = new str_data(*datap);
    refinc(datap);
  }

  if (datap->flags.get(SD_CONST)) {
    alni len = slen(datap->buff);

    char* target = datap->buff;
    datap->buff = NULL;

    datap->reserve(len);
    memcp(datap->buff, target, len);
  }
}

bool str_user::operator==(const char* cstring) const {
  if (!datap || !datap->buff || !cstring) {
    return false;
  }

  return sequal(datap->buff, cstring);
}

bool str_user::operator==(const str_user& in) const {
  return in.datap ? this->operator==(in.datap->buff) : false;
}

bool str_user::operator!=(const char* cstring) const {
  return !this->operator==(cstring);
}

bool str_user::operator!=(const str_user& in) const {
  return !this->operator==(in);
}

str_user& str_user::insert(const char* in, alni at, alni len) {
  assert_modifiable();
  alni own_len = slen(datap->buff);
  assert(own_len > at && at >= 0);
  datap->insert(in, at, len ? len : slen(in));
  return *this;
}

str_user& str_user::capture() {
  assert_modifiable();
  return *this;
}

char* str_user::get_writable() {
  assert_modifiable();
  return datap->buff;
}

void str_user::reserve(alni len) {
  if (datap->refc > 1) {
    refdec(datap);
    datap = new str_data(*datap);
    refinc(datap);
  }

  datap->reserve(len);
}

str_user& str_user::insert(const str_user& in, alni at, alni len) {
  assert(in.datap && in.datap->buff);
  insert(in.datap->buff, at);
  return *this;
}

str_user& str_user::remove(range rng) {
  assert(rng.valid());
  alni len = slen(datap->buff);
  assert(len > rng.nd && rng.nd >= 0);
  assert_modifiable();
  datap->remove(rng.st, rng.nd);
  return *this;
}

void str_user::operator=(const str_user& in) {
  if (datap) {
    refdec(datap);
  }
  datap = in.datap;
  refinc(datap);
}

void str_user::operator=(const char* in) {
  datap = new str_data(in, true);
  refinc(datap);
}

str_user& str_user::operator+=(const char* in) {
  assert_modifiable();
  datap->insert(in, slen(datap->buff), slen(in));
  return *this;
}

str_user& str_user::operator+=(const str_user& in) {
  assert(in.datap->buff);
  return operator+=(in.datap->buff);
}

str_user str_user::operator+(const str_user& in) const {
  str_user out(*this);
  return out += in;
}

str_user str_user::operator+(const char* in) const {
  str_user out(*this);
  return out += in;
}

void str_user::trim(range rng) {
  assert(rng.valid());
  alni len = slen(datap->buff);
  assert(len > rng.nd && rng.nd >= 0);
  assert_modifiable();
  datap->remove(0, rng.st);
  datap->remove(rng.nd, len);
}

alni str_user::save_size() {
  return size() + sizeof(alni);
}

void str_user::save(File* file) {
  alni len = save_size();
  file->write(&len);
  file->write_bytes(cstr(), len);
}

void str_user::load(File* file) {
  alni len;
  file->read(&len);
  reserve(len);
  file->read_bytes(get_writable(), len);
}

void str_user::clear() {
  assert_modifiable();
  datap->clear();
}

#define MAX_FORMAT_ARGUMENTS 16

str_user sfmt(const char* fmt, ...) {
  str_user out;
  alni outlen = 0;

  // chache up all sizes
  static alni sizes[MAX_FORMAT_ARGUMENTS];
  alni arg_len = 0;

  va_list args;

  // calculate size of output
  va_start(args, fmt);
  for (alni idx = 0; fmt[idx] != '\0'; idx++) {
    if (fmt[idx] != '%') {
      outlen++;
      continue;
    }
    switch (fmt[idx + 1]) {
      case 'i': {
        sizes[arg_len] = val2str_len(va_arg(args, alni), 10);
        break;
      }
      case 'f': {
        sizes[arg_len] = val2str_len(va_arg(args, alnf), 10);
        break;
      }
      case 'b': {
        sizes[arg_len] = val2str_len(va_arg(args, bool));
        break;
      }
      case 'c': {
        sizes[arg_len] = slen(va_arg(args, const char*));
        break;
      }
      case 's': {
        sizes[arg_len] = va_arg(args, string).size();
        break;
      }
      default: {
        sizes[arg_len] = 17 /* slen("$not implemented$") */;
        break;
      }
    }
    outlen += sizes[arg_len];
    arg_len++;
    idx++;
  }
  outlen++;
  va_end(args);

  out.reserve(outlen);
  char* outbuff = out.get_writable();

  alni out_idx = 0;
  alni arg_idx = 0;
  alni in_idx = 0;

  // fill up output
  va_start(args, fmt);
  for (; fmt[in_idx] != '\0'; in_idx++) {
    if (fmt[in_idx] != '%') {
      outbuff[out_idx] = fmt[in_idx];
      out_idx++;
      continue;
    }
    switch (fmt[in_idx + 1]) {
      case 'i': {
        val2str(va_arg(args, alni), &outbuff[out_idx], 10);
        break;
      }
      case 'f': {
        val2str(va_arg(args, alnf), 10, &outbuff[out_idx]);
        break;
      }
      case 'b': {
        val2str(va_arg(args, bool), &outbuff[out_idx]);
        break;
      }
      case 'c': {
        memcp(&outbuff[out_idx], va_arg(args, const char*), sizes[arg_idx]);
        break;
      }
      case 's': {
        memcp(&outbuff[out_idx], va_arg(args, string).cstr(), sizes[arg_idx]);
        break;
      }
      default: {
        memcp(&outbuff[out_idx], "$not implemented", sizes[arg_idx]);
        break;
      }
    }
    out_idx += sizes[arg_idx];
    arg_idx++;
    in_idx++;
  }
  outbuff[out_idx] = '\0';
  va_end(args);
  return out;
}