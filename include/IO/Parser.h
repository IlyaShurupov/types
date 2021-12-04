#pragma once

#include "Strings/Strings.h"
#include "Containers/List.h"

enum class DBType {
  FLOAT,
  INT,
  BOOL,
  STRING,
  COMPOUND,
  ARRAY,
};

struct DataBlock {
  Str BlockName;
  DBType type = DBType::COMPOUND;

  long long int integer = 0;
  float floating = 0;
  bool boolean = 0;
  Str string;
  List<DataBlock*> list;

  DataBlock* find(const char* BlockName);
  ~DataBlock();
};

struct DataBlock* Read_Yaml(struct Str* filepath);