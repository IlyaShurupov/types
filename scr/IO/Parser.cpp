#include "IO/Parser.h"
#include "Memory/Mem.h"

#include <cassert>
#include <fstream>
#include <sstream>

DataBlock* DataBlock::find(const char* BlockName) {
  Str name(BlockName);

  for (auto block : list) {
    if (block->BlockName == name) {
      return block.node()->data;
    }
  }
  return nullptr;
}

DataBlock::~DataBlock() {
}

void file_to_str(Str* filepath, Str& out) {

  std::ifstream file(std::string(filepath->str, filepath->len()));
  std::stringstream buffer;
  buffer << file.rdbuf();
  Str string(buffer.str().c_str());
  out = string;
  file.close();
}

int str_tab_size(Str* str, str_idx start) {
  int size = 0;
  while (str->str[start + size] == ' ') {
    size++;
  }
  return size;
}

void exclude_spaces(Str* str, Range* rng) {
  for (str_idx i = rng->strt; i < rng->end; i++) {
    if (str->str[i] == ' ') {
      rng->strt++;
      continue;
    }
    break;
  }
  for (str_idx i = rng->end; i > rng->strt; i--) {
    if (str->str[i] == ' ') {
      rng->end--;
      continue;
    }
    break;
  }

}

void write_coppound_name(DataBlock* db, Str* str, Range in) {
  exclude_spaces(str, &in);
  if (str->str[in.strt] == '-') {
    db->BlockName = "ListItem";
    return;
  }
  in.end = str->find(':', in) - 1;
  db->BlockName.coppy(str, in);
}

void write_to_db_yml(DataBlock* db, Str* str, DBType type, Range rng);
DBType dblock_type_yml(DataBlock* db, Str* str, Range* in);

void write_array_yml(DataBlock* db, Str* str, Range rng) {

  DataBlock* writeto = db;

  write_coppound_name(db, str, rng);

  rng.strt = str->find('[', rng);

  Range irange(rng);
  while ((irange.end = str->find(',', irange)) != -1) {

    DataBlock* newdb = new DataBlock();
    db->list.PushBack(newdb);

    irange.end--;
    irange.strt++;
    write_to_db_yml(newdb, str, dblock_type_yml(newdb, str, &irange), irange);
    irange.end += 2;
    irange.strt = irange.end;
    irange.end = rng.end;
  }

  DataBlock* newdb = new DataBlock();
  db->list.PushBack(newdb);
  irange.end = rng.end - 1;
  write_to_db_yml(newdb, str, dblock_type_yml(newdb, str, &irange), irange);
}

void write_to_db_yml(DataBlock* db, Str* str, DBType type, Range rng) {

  exclude_spaces(str, &rng);

  if (type == DBType::COMPOUND) {
    write_coppound_name(db, str, rng);
    return;

  } else if (type == DBType::ARRAY) {
    write_array_yml(db, str, rng);
    return;
  }

  str_idx assign = str->find(':', rng) - 1;
  if (assign == -2) {
    db->BlockName = "UNNAMED";
  } else {
    db->BlockName.coppy(str, Range(rng.strt, assign));
    rng.strt = assign + 2;
  }

  exclude_spaces(str, &rng);

  switch (type) {

    case DBType::STRING: {
      db->string.coppy(str, Range(rng.strt + 1, rng.end - 1));
      return;
    }

    case DBType::BOOL: {
      db->boolean = str->str[rng.strt] == 't' ? 1 : 0;
      return;
    }

    case DBType::FLOAT: {
      Str flt;
      flt.coppy(str, rng);
      db->floating = std::stof(std::string(flt.str));
      return;
    }

    case DBType::INT: {
      Str integer;
      integer.coppy(str, rng);
      db->integer = std::stoll(std::string(integer.str));
      return;
    }
  }
}

DBType dblock_type_yml(DataBlock* db, Str* str, Range* in) {

  exclude_spaces(str, in);

  switch (str->str[in->end]) {
    case ':':
    case '-':
      return db->type = DBType::COMPOUND;

    case 'e':
    case 's':
      return db->type = DBType::BOOL;

    case '"':
      return db->type = DBType::STRING;

    case ']':
      return db->type = DBType::ARRAY;

    default:
      for (str_idx i = in->strt; i < in->end; i++) {
        if (str->str[i] == '.') {
          return db->type = DBType::FLOAT;
        }
      }
      return db->type = DBType::INT;
  }

  return db->type = DBType::BOOL;
}

bool find_dblock_yml(Str* str, Range in, Range& out) {

  out = in;
  bool keep = true;

  while (keep) {

    out.end = str->find('\n', in);
    if (out.end == -1) {
      return false;
    }

    if (out.end == out.strt) {
      out.strt = out.end + 1;
      in.strt = out.end + 1;
      continue;

    } else {

      bool nonempthy = false;
      for (str_idx i = out.strt; i < out.end; i++) {
        if (str->str[i] != ' ') {
          nonempthy = true;
          break;
        }
      }

      if (!nonempthy) {
        out.strt = out.end + 1;
        in.strt = out.end + 1;
        continue;
      }
    }
    keep = false;
  }

  out.end--;
  return true;
}

int read_dblock_yml(DataBlock* prnt, Str* str, char coloum, Range* in) {

  Range dbrange;
  bool keep = true;
  int clm;

  while (find_dblock_yml(str, *in, dbrange)) {

    clm = str_tab_size(str, dbrange.strt);

    if (clm <= coloum) {
      return clm;
    }

    in->strt = dbrange.end + 2;

    DataBlock* newdb = new DataBlock();
    DBType dbtype = dblock_type_yml(newdb, str, &dbrange);

    prnt->list.PushBack(newdb);

    write_to_db_yml(newdb, str, dbtype, dbrange);

    if (dbtype == DBType::COMPOUND) {
      if (read_dblock_yml(newdb, str, clm, in) < clm) {
        return clm;
      }
    }
  }

  return 0;
}

DataBlock* Read_Yaml(Str* filepath) {
  Str file;
  file_to_str(filepath, file);

  if (file.str[file.length - 1] != '\n') {
    file += Str("\n");
  }

  DataBlock* dblock = new DataBlock();
  dblock->BlockName.coppy(filepath, Range(filepath->rfind('\\', Range(0, filepath->length)) + 1, filepath->length));
  Range filerange = Range(0, file.length);
  read_dblock_yml(dblock, &file, -1, &filerange);
  return dblock;
}
