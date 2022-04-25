
#include "topology.h"

#define MATH_EPSILON 0.00001

vec3f camera::get_target() {
	return pos + tmat.K * targetlnegth;
}

void camera::lookat(vec3f target, vec3f pos, vec3f up) {
	if (target == pos) {
		return;
	}
	up.normalize();

	camera prev_cfg(*this);
	this->pos = pos;
	targetlnegth = (halnf) (target - pos).length();
	tmat.K = (target - pos) / targetlnegth;
	tmat.J = tmat.K.cross(up);
	tmat.I = tmat.J.cross(tmat.K);

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
	normalized *= 2;
	halnf scale = (halnf) (trigs::tan(fov / 2) * targetlnegth);
	vec3f offset = (tmat.J * normalized.x * scale * ratio) + (tmat.I * normalized.y * scale);
	return get_target() + offset;
}

vec2f camera::project(vec3f world) {
	typedef vec<halnf, 4> vec4;
	vec4 world_pos4;
	world_pos4[0] = world.x;
	world_pos4[1] = world.y;
	world_pos4[2] = world.z;
	world_pos4[3] = 1;

	vec4 transformed = (projmat() * viewmat()) * world_pos4;
	return vec2f(transformed[0] / transformed[3], transformed[1] / transformed[3]);
}

void camera::zoom(halnf ratio) {
	
	ratio = ABS(ratio);
	
	if (ratio < 0.01 || abs(targetlnegth) < 0.1) {
		return;
	}

	vec3f target = get_target();
	pos = target + (pos - target) * ratio;
	lookat(target, pos, tmat.I);
	updated = true;
}

void camera::move(vec2f mpos, vec2f mprevpos) {
	vec3f target = get_target();
	vec3f p1 = project(mprevpos);
	vec3f p2 = project(mpos);
	vec3f move = p1 - p2;
	pos += move;
	target += move;

	lookat(target, pos, tmat.I);
	updated = true;
}

void camera::rotate(halnf anglex, halnf angley) {
	vec3f up(0, 0, 1);
	vec3f target = get_target();
	pos -= target;

	pos = mat3f::rotmat(up, anglex) * pos;
	tmat.J = (-pos).unitv().cross(up).unitv();
	pos = mat3f::rotmat(tmat.J, -angley) * pos;
	
	tmat.K = (-pos).unitv();
	tmat.I = tmat.J.cross(tmat.K);

	pos += target;
	updated = true;
}

mat<halnf, 4, 4> camera::projmat() {
	mat<halnf, 4, 4> out(0);
	halnf const tanHalfFovy = (halnf) trigs::tan(fov / 2.f);
	out[0][0] = ratio / tanHalfFovy;
	out[1][1] = 1 / (tanHalfFovy);
	out[2][2] = -(far + near) / (far - near);
	out[2][3] = -1;
	out[3][2] = -(2 * far * near) / (far - near);
	return out;
}

mat<halnf, 4, 4> camera::viewmat() {
	mat<halnf, 4, 4> out;

	vec3f const f((get_target() - pos).unitv());
	vec3f const s((f.cross(tmat.I)).unitv());
	vec3f const u(s.cross(f));

	out[0][0] = s.x;
	out[1][0] = s.y;
	out[2][0] = s.z;

	out[0][1] = u.x;
	out[1][1] = u.y;
	out[2][1] = u.z;
	
	out[0][2] = -f.x;
	out[1][2] = -f.y;
	out[2][2] = -f.z;
	
	out[0][3] = 0;
	out[1][3] = 0;
	out[2][3] = 0;
	out[3][3] = 1;

	out[3][0] = -s.dot(pos);
	out[3][1] = -u.dot(pos);
	out[3][2] = f.dot(pos);

	return out;
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
	basis.transform(vert);
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