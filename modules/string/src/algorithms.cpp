
#include "algorithms.h"

alni slen(const char* in) {
	alni out = 0;
	for (const char* iter = in; *iter != '\0'; iter++) {
		out++;
	}
	return out;
}

char* sinsert(const char* cur, const char* tar, alni atidx, alni len) {
	alni cur_len = slen(cur);
	alni all_len = cur_len + len;
	char* out = new char[all_len + 1];

	for (alni idx = 0; idx < atidx; idx++) {
		out[idx] = cur[idx];
	}

	for (alni idx = 0; idx < len; idx++) {
		out[idx + atidx] = tar[idx];
	}

	for (alni idx = atidx + len; idx <= all_len; idx++) {
		out[idx] = cur[idx - len];
	}

	return out;
}

void soverride(char* cur, const char* tar, alni atidx, alni len) {
	for (alni idx = 0; idx < len; idx++) {
		cur[idx + atidx] = tar[idx];
	}
}

char* sremove(const char* cur, alni start, alni end) {
	alni cur_len = slen(cur);
	alni range_len = end - start;
	alni new_len = cur_len - range_len;
	char* out = new char[new_len + 1];
	out[new_len] = '\0';

	for (alni idx = 0; idx < start; idx++) {
		out[idx] = cur[idx];
	}

	for (alni idx = end; idx < cur_len; idx++) {
		out[idx - range_len] = cur[idx];
	}

	return out;
}

alni sequal(const char* left, const char* right) {
  alni idx = 0;

LOOP:
  if (left[idx] == '\0' || right[idx] == '\0') {
    if (left[idx] == '\0' && right[idx] == '\0') {
      return true;
    }
    return false;
  }
  if (left[idx] != right[idx]) {
    return false;
  }
  idx++;
  goto LOOP;

  return false;
}

char* val2srt(alni val, char* ownbuff, alni base) {
  char* out = NULL;

  alni iter = val;
  alni len = 0;
  while (iter /= base) {
    len++;
  }

  bool neg = val < 0;

  len += 1 + short(neg);

  out = ownbuff ? ownbuff : new char[len + 1];
  out[len] = '\0';

  val = ABS(val);

  for (alni i = len - 1; i >= int(neg); i--) {
    out[i] = (char)(val % base + 48);
    val /= base;
  }

  if (neg) {
    out[0] = '-';
  }

  return out;
}

char* val2srt(alnf val, alni base) {
  char* out = NULL;

  alni rounded = (alni)val;
  alni mantissa = (alni)(val - rounded) * 100000;
  
  alni rounded_len = 0;
  alni mantissa_len = 0;

  while (rounded /= base) {
    rounded_len++;
  }
  if (!rounded_len) {
    rounded_len++;
  }

  while (mantissa /= base) {
    mantissa_len++;
  }

  bool neg = val < 0;
  bool dot = mantissa_len & 1;
  alni tot_len = mantissa_len + rounded_len + dot + neg;

  out = new char[tot_len + 1];
  out[tot_len] = '\0';

  if (neg) {
    out[0] = '-';
  }
  
  if (dot) {
    out[rounded_len + neg] = '.';
  }

  val2srt(rounded, out + neg);
  if (mantissa_len) {
    val2srt(mantissa, out + rounded_len + neg + dot);
  }

  return out;
}

char* val2srt(bool val) {
  alni len = val ? 4 : 5;
  char* out = new char[len];
  out[len] = '\0';

  memcp(out, val? "True" : "False", len);

  return out;
}

char* val2srt(int val, alni base) {
  return val2srt((alni)val, NULL, base);
}

char* val2srt(char val) {
  char* out = new char[2];
  out[1] = '\0';
  out[0] = val;
  return out;
}


bool str2val(const char* in, alni& val, alni base, alni ownlen)   {
  val = 0;
  alni len = ownlen? ownlen :slen(in);
  bool neg = in[0] == '-';

  for (alni idx = len - 1; idx >= neg; idx--) {
    if ((int1)in[idx] < 48 || (int1)in[idx] > 48 + base - 1) {
      return false;
    }

    val *= base;
    val += (int1)in[idx] - 48;
  }

  if (neg) val *= -1;
  return true;
}

bool str2val(const char* in, alnf& val, alni base) {
  alni len = slen(in);
  alni dot_idx = -1;
  
  for (alni idx = 0; idx < len; idx++) {
    if (in[idx] == '.') {
      dot_idx = idx;
      break;
    }
  }

  bool ok = true;
  alni rounded = 0;
  alni manissia = 0;

  if (dot_idx > 0) {
    ok &= str2val(in + dot_idx, manissia, base, 0);
    ok &= str2val(in, rounded, base, dot_idx);
  }
  else {
    ok &= str2val(in, rounded, base, 0);
  }


  val = rounded + manissia / (len - dot_idx);
  return ok;
}

bool str2val(const char* in, bool& val) {
  alni length = slen(in);

  if (length == 0) {
    return false;
  }

  if (sequal(in, "False") || sequal(in, "false") || sequal(in, "0")) {
    val = 0;
  }
  else {
    val = 1;
  }

  return true;
}