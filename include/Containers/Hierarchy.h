#pragma once
#include "Strings/Strings.h"
#include "Containers/List.h"

// idx tmpl param is the idx of hrchy member in the struct
template <typename Type, typename ListType, int hrch_idx>
struct Hierarchy {

  Str id;
  Type* prnt = nullptr;
  ListType childs;

  Hierarchy() {}
  Hierarchy(Type* parent) {
    join(parent);
  }

  void add(Type* chld) {
    childs.add(chld);
    ((Hierarchy*)chld + hrch_idx)->prnt = (Type*)(this - hrch_idx);
  }

  void join(Type* parent) {
    ((Hierarchy*)parent + hrch_idx)->childs.PushBack((Type*)(this - hrch_idx));
    this->prnt = parent;
  }

  void leave() {
    if (prnt) {
      Node<Type>* node = ((Hierarchy*)prnt + hrch_idx)->childs.Find((Type*)(this - hrch_idx));
      ((Hierarchy*)prnt + hrch_idx)->childs.Detach(node);
    }
  }

  Type* root() {
    Hierarchy* root = (Hierarchy*)prnt;
    while (true) {
      if ((root + hrch_idx)->prnt) {
        root = (Hierarchy*)(root + hrch_idx)->prnt;
        continue;
      }
      break;
    }
    return (Type*)root;
  }
};

template <typename Type>
using Hrchy = Hierarchy<Type, List<Type>, 0>;