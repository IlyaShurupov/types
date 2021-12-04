
#pragma once

#include "Memory/Mem.h"

template <typename Type>
bool compare(const Type& val1, const Type& val2) {
  return val1 > val2;
}

struct SortMerge {

  template <typename Type>
  static void Sort(Type* pxlbuff, int length, bool (*grater)(const Type& obj1, const Type& obj2) = &compare) {
    mergeSort(pxlbuff, 0, length - 1, grater);
  }

 private:

  template <typename Type>
  static void merge(Type* pxlbuff, int left, int middle, int right, bool (*grater)(const Type& obj1, const Type& obj2)) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    Type* Left = new Type[n1];
    Type* Right = new Type[n2];

    for (int i = 0; i < n1; i++) {
      Left[i] = pxlbuff[left + i];
    }

    for (int j = 0; j < n2; j++) {
      Right[j] = pxlbuff[middle + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
      if (!(grater(Left[i], Right[j]))) {
        pxlbuff[k] = Left[i];
        i++;
      } else {
        pxlbuff[k] = Right[j];
        j++;
      }
      k++;
    }

    while (i < n1) {
      pxlbuff[k] = Left[i];
      i++;
      k++;
    }

    while (j < n2) {
      pxlbuff[k] = Right[j];
      j++;
      k++;
    }

    delete Left;
    delete Right;
  }

  template <typename Type>
  static void mergeSort(Type* pxlbuff, int left, int right, bool (*grater)(const Type& obj1, const Type& obj2)) {

    if (left >= right) {
      return;
    }

    int middle = left + (right - left) / 2;
    
    mergeSort(pxlbuff, left, middle, grater);
    mergeSort(pxlbuff, middle + 1, right, grater);
    merge(pxlbuff, left, middle, right, grater);
  }

};

struct SortInsert {
  template <typename Type>
  static void Sort(Type* pxlbuff, int length, bool (*grater)(const Type& obj1, const Type& obj2) = &compare) {
    for (int i = 0; i < length; i++) {
      for (int j = i + 1; j < length; j++) {
        if (grater(*pxlbuff[i], *pxlbuff[j])) {
          SWAP(pxlbuff[i], pxlbuff[j], Type*);
        }
      }
    }
  }
};

struct SortQuic {
  static void Sort(void* pxlbuff, char itemsize, char offset) {
  }
};

struct SortRadix {
  static void Sort(void* pxlbuff, char itemsize, char offset) {
  }
};

struct SortCount {
  static void Sort(void* pxlbuff, char itemsize, char offset) {
  }
};