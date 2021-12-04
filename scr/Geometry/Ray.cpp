#include "Geometry/Ray.h"
#include "Geometry/Mesh.h"

Ray::Ray() {
}

Ray::Ray(Vec3f& Dir, Vec3f& Pos) {
	this->Dir = Dir;
	this->Pos = Pos;
	HitData.Hit = false;
}

Ray::~Ray() {
}

void WorldTransform(Trig* trig, Vec3f* pos, Mat3f* mat) {

	mat->Transform(trig->V0);
	mat->Transform(trig->V1);
	mat->Transform(trig->V2);

	trig->V0 + *pos;
	trig->V1 + *pos;
	trig->V2 + *pos;
}

void Ray::Cast(List<Mesh*>* objects, float ray_length) {
	HitData.Hit = false;

	for (auto mesh : *objects) {
		for (auto trig : mesh->Trigs) {

			WorldTransform(trig.node()->data, &mesh->Pos, &mesh->TransformMat);

			Vec3f HitPos;
			if (!trig->RayHit(*this, HitPos)) {
				continue;
			}

			float dist = (float)(HitPos - Pos).Length();
			if (dist < ray_length) {
				
				HitData.Hit = true;
        		HitData.Obj = mesh.node()->data;
				HitData.trig = trig.node()->data;
				HitData.HitPos = HitPos;

				ray_length = dist;
			}
		}
	}
}

