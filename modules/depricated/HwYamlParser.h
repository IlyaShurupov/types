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
  string BlockName;
  DBType type = DBType::COMPOUND;

  long long int integer = 0;
  float floating = 0;
  bool boolean = 0;
  string str;
  List<DataBlock*> list;

  DataBlock* find(const char* BlockName);
  ~DataBlock();
};

struct DataBlock* Read_Yaml(class string* filepath);