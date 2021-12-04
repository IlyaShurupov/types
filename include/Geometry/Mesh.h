#pragma once 

#include "Containers/List.h"
#include "Matrix.h"
#include "Trig.h"

class Mesh {
public:
  Mesh();
 ~Mesh();
  Mat3f TransformMat;
  Vec3f Pos;
	List<Trig*> Trigs;
};