#pragma once

#include "Vec3.h"

template <typename Type> class mat3;
using mat3f = mat3<halnf>;

template <typename Type>
class mat3 {
	typedef vec3<Type> vec;

	public:

	vec I;
	vec J;
	vec K;

	public:

	mat3() {
		I.assign(1.f, 0.f, 0.f);
		J.assign(0.f, 1.f, 0.f);
		K.assign(0.f, 0.f, 1.f);
	}

	mat3(const vec& i, const vec& j, const vec& k) {
		I = i;
		J = j;
		K = k;
	}
	mat3(const mat3& Matx) {
		this->I = Matx.I;
		this->J = Matx.J;
		this->K = Matx.K;
	}
	void assign(const vec& i, const vec& j, const vec& k) {
		I = i;
		J = j;
		K = k;
	}

	// create on stack
	mat3 operator+(const mat3& Matx) { return mat3(this->I + Matx.I, this->J + Matx.J, this->K + Matx.K); }
	mat3 operator-(const mat3& Matx) { return mat3(this->I - Matx.I, this->J - Matx.J, this->K - Matx.K); }
	mat3 operator+(Type val) { return mat3(this->I + val, this->J + val, this->K + val); }
	mat3 operator-(Type val) { return mat3(this->I - val, this->J - val, this->K - val); }
	mat3 operator*(Type val) { return mat3(this->I * val, this->J * val, this->K * val); }
	mat3 operator/(Type val) { return mat3(this->I / val, this->J / val, this->K / val); }

	// write
	void operator=(const mat3& Matx) {
		this->I = Matx.I;
		this->J = Matx.J;
		this->K = Matx.K;
	}
	void operator+=(const mat3& Matx) {
		this->I += Matx.I;
		this->J += Matx.J;
		this->K += Matx.K;
	}
	void operator-=(const mat3& Matx) {
		this->I -= Matx.I;
		this->J -= Matx.J;
		this->K -= Matx.K;
	}
	void operator+=(Type val) {
		this->I += val;
		this->J += val;
		this->K += val;
	}
	void operator-=(Type val) {
		this->I -= val;
		this->J -= val;
		this->K -= val;
	}
	void operator*=(Type val) {
		this->I *= val;
		this->J *= val;
		this->K *= val;
	}
	void operator/=(Type val) {
		this->I /= val;
		this->J /= val;
		this->K /= val;
	}

	// Matrix transformation

	vec& Transform(vec& vec) {
		Type x = I.x * vec.x + J.x * vec.y + K.x * vec.z;
		Type y = I.y * vec.x + J.y * vec.y + K.y * vec.z;
		Type z = I.z * vec.x + J.z * vec.y + K.z * vec.z;
		vec.assign(x, y, z);
		return vec;
	}

	mat3& Transform(mat3& Matx) {
		Matx.assign(this->Transform(Matx.I), this->Transform(Matx.J), this->Transform(Matx.K));
		return Matx;
	}

	mat3 operator*(const mat3& in) {
		mat3 out;
		Transform(out);
		return out;
	}

	void Rotate(vec Dir, alnf angle) {

		alnf cosa = (alnf) trigs::cos(angle);
		alnf sina = (alnf) trigs::sin(angle);
		alnf tmp = 1 - cosa;

		I.x = cosa + Dir.x * Dir.x * tmp;
		I.y = Dir.y * Dir.x * tmp + Dir.z * sina;
		I.z = Dir.z * Dir.x * tmp - Dir.y * sina;

		J.x = Dir.x * Dir.y * tmp + Dir.z * sina;
		J.y = cosa + Dir.y * Dir.y * tmp;
		J.z = Dir.z * Dir.y * tmp + Dir.x * sina;

		K.x = Dir.x * Dir.z * tmp + Dir.y * sina;
		K.y = Dir.y * Dir.z * tmp + Dir.x * sina;
		K.z = cosa + Dir.z * Dir.z * tmp;
	}
};
