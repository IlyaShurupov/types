#pragma once 

#include "list.h"
#include "matrix.h"
#include "trig.h"

class Mesh {
public:
	Mesh();
	~Mesh();
	mat3f TransformMat;
	vec3f Pos;
	list<Trig*> Trigs;
};