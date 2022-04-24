
#include "topology.h"

#define MATH_EPSILON 0.00001

vec3f camera::get_target() {
	return pos + mat.K * targetlnegth;
}

void camera::lookat(vec3f target, vec3f pos, vec3f up) {
	if (target == pos) {
		return;
	}
	up.normalize();

	camera prev_cfg(*this);
	this->pos = pos;
	targetlnegth = (halnf) (target - pos).length();
	mat.K = (target - pos) / targetlnegth;
	mat.J = mat.K.cross(up);
	mat.I = mat.J.cross(mat.K);

	if (!memequal(&prev_cfg, this, sizeof(camera))) {
		updated = true;
	}
}

void camera::set_ratio(halnf ratio) {
	if (this->ratio != ratio) {
		this->ratio = ratio;
		updated = true;
	}
}

void camera::set_fov(halnf fov) {
	if (this->fov != fov) {
		this->fov = fov;
		updated = true;
	}
}

vec3f camera::project(vec2f normalized) {
	vec3f target = get_target();
	halnf scale = (halnf) (trigs::tan(fov / 2) * (target - pos).length());
	vec3f out = target + (mat.J * normalized.x * scale * ratio) + (mat.I * normalized.y * scale);
	return out;
}

vec2f camera::project(vec3f world) {
	//vec4 transformed = (projmat() * viewmat()) * vec4(world, 1);
	//vec2 screen_pos = vec2(transformed.x / transformed.w, transformed.y / transformed.w);
	vec2f screen_pos;
	return screen_pos;
}

void camera::zoom(halnf ratio) {
	
	ratio = ABS(ratio);
	
	if (ratio < 0.01 || abs(targetlnegth) < 0.1) {
		return;
	}

	vec3f target = get_target();
	pos = target + (pos - target) * ratio;
	lookat(target, pos, mat.I);
	updated = true;
}

void camera::move(vec2f mpos, vec2f mprevpos) {
	vec3f target = get_target();
	vec3f p1 = project(mprevpos);
	vec3f p2 = project(mpos);
	vec3f move = p1 - p2;
	pos += move * 2.f;
	target += move * 2.f;

	lookat(target, pos, mat.I);
	updated = true;
}

void camera::rotate(halnf anglex, halnf angley) {
	vec3f up(0, 0, 1);
	vec3f target = get_target();
	pos -= target;

	pos = pos.RotateAround(up, anglex);

	mat.K = (-pos).unitv();
	mat.J = mat.K.cross(up).unitv();
	mat.I = mat.J.cross(mat.K);

	pos = pos.RotateAround(mat.J, -angley);

	mat.K = (-pos).unitv();
	mat.I = mat.J.cross(mat.K);

	pos += target;
	updated = true;
}

vec3f indexed_trig::HitPos;

indexed_trig::indexed_trig() : v1(0), v2(0), v3(0) {
}
indexed_trig::indexed_trig(halni v1, halni v2, halni v3) : v1(v1), v2(v2), v3(v3) {
}

void indexed_trig::update_cache(Array<vec3f>& points) {
	v1_val = points[v1];
	edge1 = points[v2] - points[v1];
	edge2 = points[v3] - points[v1];
	normal = (points[v2] - points[v1]).cross(points[v3] - points[v1]).unitv();
}

bool indexed_trig::RayHit(const Ray& ray) {
	static vec3f h, s, q;
	static halnf a, f, u, v;
	static halnf t;

	h = ray.Dir.cross(edge2);
	a = edge1.dot(h);

	if (a > -MATH_EPSILON && a < MATH_EPSILON) {
		return false;
	}

	f = 1.f / a;
	s = ray.Pos - v1_val;
	u = f * s.dot(h);

	if (u < 0.0 || u > 1.0) {
		return false;
	}

	q = s.cross(edge1);
	v = f * ray.Dir.dot(q);

	if (v < 0.f || u + v > 1.f) {
		return false;
	}

	t = f * edge2.dot(q);
	if (t > MATH_EPSILON) {
		HitPos = ray.Pos + ray.Dir * t;
		return true;
	} else {
		return false;
	}
}

vec3f& indexed_trig::get_hit_pos() {
	return HitPos;
}

vec3f indexed_trig::get_normal() {
	return normal;
}

void topology::add_trig(vec3f v1, vec3f v2, vec3f v3) {

	halni trig_idx = (halni) points.Len();
	Array<vec3f> newpoints(3);
	newpoints[0] = v1;
	newpoints[1] = v2;
	newpoints[2] = v3;

	points += newpoints;

	transform_point(newpoints[0]);
	transform_point(newpoints[1]);
	transform_point(newpoints[2]);

	transformed_points += newpoints;

	trig newtrig(trig_idx, trig_idx + 1, trig_idx + 2);
	newtrig.update_cache(transformed_points);
	trigs.PushBack(newtrig);
}

void topology::transform_point(vec3f& vert) {
	basis.Transform(vert);
	vert += origin;
}

void topology::update_transformed() {
	transformed_points = points;
	for (alni idx : range(transformed_points.Len())) {
		transform_point(transformed_points[idx]);
	}
	for (alni idx : range(trigs.Len())) {
		trigs[idx].update_cache(transformed_points);
	}
}