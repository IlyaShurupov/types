
#include "topology.h"

#define MATH_EPSILON 0.00001

namespace tp {

	Camera::Camera() {
		lookat(vec3f(0, 0, 0), vec3f(2, 0, 0), vec3f(0, 0, 1));
	}

	mat4f Camera::transform_mat() {
		return projmat() * viewmat();
	}

	mat4f Camera::viewmat() {
		const vec3f& F = (pos - target).unitv();
		const vec3f& S = up * F;
		const vec3f& U = F * S;
		const vec3f& P = pos;

		mat4f out;
		out[0] = vec4f(S.x, S.y, S.z, -P.dot(S));
		out[1] = vec4f(U.x, U.y, U.z, -P.dot(U));
		out[2] = vec4f(F.x, F.y, F.z, -P.dot(F));
		out[3] = vec4f(0, 0, 0, 1);
		return out;
	}

	mat4f Camera::projmat() {
		halnf r = (halnf) sqrt(ratio);
		halnf c = 1 / r;
		halnf s = halnf(1.f / tan(fov / 2.f));

		mat4f out;
		out[0] = vec4f(s * r, 0, 0, 0);
		out[1] = vec4f(0, s * c, 0, 0);
		out[2] = vec4f(0, 0, -2.f / (far - near), -(far + near) / (far - near));
		out[3] = vec4f(0, 0, -1, 0);
		return out;
	}

	vec3f Camera::project(vec2f normalized) {
		auto cammat = transform_mat();
		auto inv = cammat.inv();

		halnf z = halnf((((target - pos).length() - near) / (far - near) - 1.f / 2) * 2.f);
		halnf w = halnf((target - pos).length());
		vec4<halnf> world_pos4(normalized.x * w, normalized.y * w, z, w);

		auto out = inv * world_pos4;
		vec2f tmp = project(vec3f(out));
		return vec3f(out);
	}

	vec2f Camera::project(vec3f world) {
		vec4f world_pos4(world.x, world.y, world.z, 1);
		vec4f transformed = viewmat() * world_pos4;
		transformed = projmat() * transformed;
		return vec2f(transformed[0] / transformed[3], transformed[1] / transformed[3]);
	}

	void Camera::lookat(vec3f p_target, vec3f p_pos, vec3f p_up) {
		if (p_target == p_pos) {
			return;
		}
		pos = p_pos;
		target = p_target;
		vec3f f = (pos - target).normalize();
		up = f * (p_up.normalize() * f);
	}

	void Camera::zoom(halnf ratio) {
		ratio = ABS(ratio);
		if (ratio < 0.1 || abs((pos - target).length2()) < 0.1) {
			return;
		}
		pos = target + (pos - target) * ratio;
		lookat(target, pos, up);
	}

	void Camera::move(vec2f mpos, vec2f mprevpos) {
		vec3f p1 = project(mprevpos);
		vec3f p2 = project(mpos);
		vec3f move = p1 - p2;
		pos += move;
		target += move;
		lookat(target, pos, up);
	}

	void Camera::rotate(halnf anglex, halnf angley) {
		vec3f wup(0, 0, 1);
		pos -= target;

		mat3f rotz = mat3f::rotmat(wup, anglex);
		pos = rotz * pos;
		up = rotz * up;

		vec3f f = pos.unitv();
		vec3f s = up * f;

		pos = mat3f::rotmat(s, -angley) * pos;

		pos += target;

		lookat(target, pos, up);
	}

	vec3f Camera::get_target() {
		return target;
	}

	void Camera::offset_target(halnf val) {
		target += (pos - target).normalize() * val;
	}

	vec3f Camera::get_fw() {
		return (target - pos).normalize();
	}

	vec3f Camera::get_up() {
		return up;
	}

	vec3f& Camera::get_pos() {
		return pos;
	}

	void Camera::set_ratio(halnf ratio) {
		this->ratio = ratio;
	}

	void Camera::set_fov(halnf fov) {
		this->fov = fov;
	}

	halnf Camera::get_fov() {
		return this->fov;
	}

	alni Camera::sizeAllocatedMem() {
		return sizeof(halnf) * 4 + pos.sizeAllocatedMem() + target.sizeAllocatedMem() + up.sizeAllocatedMem();
	}

	alni Camera::sizeUsedMem() {
		return sizeof(halnf) * 4 + pos.sizeUsedMem() + target.sizeUsedMem() + up.sizeUsedMem();
	}

	vec3f IndexedTrig::HitPos;

	IndexedTrig::IndexedTrig() : v1(0), v2(0), v3(0) {}
	IndexedTrig::IndexedTrig(halni v1, halni v2, halni v3) : v1(v1), v2(v2), v3(v3) {}

	void IndexedTrig::update_cache(Array<vec3f>& points) {
		v1_val = points[v1];
		edge1 = points[v2] - points[v1];
		edge2 = points[v3] - points[v1];
		normal = (points[v2] - points[v1]).cross(points[v3] - points[v1]).unitv();
	}

	bool IndexedTrig::rayHit(const Ray& ray) {
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

	vec3f& IndexedTrig::get_hit_pos() {
		return HitPos;
	}

	vec3f IndexedTrig::get_normal() {
		return normal;
	}

	alni IndexedTrig::sizeAllocatedMem() {
		alni out = sizeof(halnf) * 3;
		out += edge1.sizeAllocatedMem() + edge2.sizeAllocatedMem() + v1_val.sizeAllocatedMem() + normal.sizeAllocatedMem();
		return out;
	}

	alni IndexedTrig::sizeUsedMem() {
		alni out = sizeof(halnf) * 3;
		out += edge1.sizeUsedMem() + edge2.sizeUsedMem() + v1_val.sizeUsedMem() + normal.sizeUsedMem();
		return out;
	}

	void topology::add_trig(vec3f v1, vec3f v2, vec3f v3) {

		halni trig_idx = (halni) points.length();
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
		trigs.pushBack(newtrig);
	}

	void topology::transform_point(vec3f& vert) {
		basis.transform(vert);
		vert += origin;
	}

	void topology::update_transformed() {
		transformed_points = points;
		for (alni idx : Range(transformed_points.length())) {
			transform_point(transformed_points[idx]);
		}
		for (alni idx : Range(trigs.length())) {
			trigs[idx].update_cache(transformed_points);
		}
	}

	alni topology::sizeAllocatedMem() {
		alni out = 0;
		out += points.sizeAllocatedMem();
		out += transformed_points.sizeAllocatedMem();
		out += trigs.sizeAllocatedMem();
		out += basis.sizeAllocatedMem();
		out += origin.sizeAllocatedMem();
		return out;
	}

	alni topology::sizeUsedMem() {
		alni out = 0;
		out += points.sizeUsedMem();
		out += transformed_points.sizeUsedMem();
		out += trigs.sizeUsedMem();
		out += basis.sizeUsedMem();
		out += origin.sizeUsedMem();
		return out;
	}

};