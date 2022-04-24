#pragma once

#include "trigonometry.h"

template <typename Type> class vec3;
using vec3f = vec3<halnf>;
using vec3i = vec3<halni>;

template <typename Type>
class vec3 {
	public:

	Type x;
	Type y;
	Type z;

	// Initialization

	vec3(Type x, Type y, Type z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	vec3(Type vec[3]) {
		x = vec[0];
		y = vec[1];
		z = vec[2];
	}
	vec3() {
		x = y = z = 0;
	}
	vec3(Type x) {
		assign(x);
	}
	vec3(const vec3& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	void assign(Type x, Type y, Type z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void assign(const vec3& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	void assign(Type val) {
		x = val;
		y = val;
		z = val;
	}
	vec3& operator=(const vec3& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	vec3& operator=(const Type* in) {
		x = in[0];
		y = in[1];
		z = in[2];
		return *this;
	}

	//  create on stack
	vec3 operator+(const vec3& vec) const {
		return vec3(x + vec.x, y + vec.y, z + vec.z);
	}
	vec3 operator-(const vec3& vec) const {
		return vec3(x - vec.x, y - vec.y, z - vec.z);
	}
	vec3 operator+(Type val) const {
		return vec3(x + val, y + val, z + val);
	}
	vec3 operator-(Type val) const {
		return vec3(x - val, y - val, z - val);
	}
	vec3 operator*(Type val) const {
		return vec3(x * val, y * val, z * val);
	}
	vec3 operator/(Type val) const {
		return vec3(x / val, y / val, z / val);;
	}


// compare
	bool operator>(const vec3<Type>& vec) const {
		return length2() > vec.length2();
	}
	bool operator<(const vec3<Type>& vec) const {
		return length2() < vec.length2();
	}
	bool operator>=(const vec3<Type>& vec) const {
		return length2() >= vec.length2();
	}
	bool operator<=(const vec3<Type>& vec) const {
		return length2() <= vec.length2();
	}
	bool operator==(const vec3<Type>& vec) const {
		return (x == vec.x && y == vec.y && z == vec.z);
	}

//  write
	vec3 operator-() {
		return vec3(-x, -y, -z);
	}
	void operator -= (Type val) {
		x -= val;
		y -= val;
		z -= val;
	}
	void operator += (Type val) {
		x += val;
		y += val;
		z += val;
	}
	void operator -= (const vec3& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}
	void operator += (const vec3& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}
	void operator *= (Type val) {
		x *= val;
		y *= val;
		z *= val;
	}
	void operator /= (Type val) {
		x /= val;
		y /= val;
		z /= val;
	}

	// Vector Properties

	Type dot(const vec3& vec) const {
		return (x * vec.x + y * vec.y + z * vec.z);
	}
	alnf length() const {
		return trigs::sqrt((halnf) (x * x + y * y + z * z));
	}
	Type length2() const {
		return (x * x + y * y + z * z);
	}
	vec3 unitv() const {
		return *this / (Type) this->length();
	}
	void normalize() {
		*this /= (Type) this->length();
	}
	vec3 cross(const vec3& vec) const {
		Type x = this->y * vec.z - this->z * vec.y;
		Type y = this->z * vec.x - this->x * vec.z;
		Type z = this->x * vec.y - this->y * vec.x;
		return vec3(x, y, z);
	}

	// Vector Transformation

	void RotZ(alnf cosa, alnf sina) {
		Type tmp = x;
		x = (Type) (x * cosa - y * sina);
		y = (Type) (tmp * sina + y * cosa);
	}
	void RotY(float cosa, float sina) {
		Type tmp = x;
		x = (Type) (x * cosa - z * sina);
		z = (Type) (tmp * sina + z * cosa);
	}
	void RotX(float cosa, float sina) {
		Type tmp = y;
		y = (Type) (y * cosa - z * sina);
		z = (Type) (tmp * sina + z * cosa);
	}

	vec3<Type> RotateAround(const vec3<Type>& axis, halnf angle) {
		vec3f out(*this);

		halnf angl_x = axis.anglex();

		halnf sin_angl_x = (halnf) trigs::sin(angl_x);
		halnf cos_angl_x = (halnf) trigs::cos(angl_x);

		halnf angl_y = axis.angley();

		halnf sin_angl_y = (halnf) trigs::sin(angl_y);
		halnf cos_angl_y = (halnf) trigs::cos(angl_y);

		out.RotX(cos_angl_x, sin_angl_x);
		out.RotY(cos_angl_y, sin_angl_y);
		out.RotZ(trigs::cos(angle), trigs::sin(angle));
		out.RotY(cos_angl_y, -sin_angl_y);
		out.RotX(cos_angl_x, -sin_angl_x);

		return out;
	}

	halnf anglex() const {
		return (halnf) trigs::atan2(y, z);
	}

	halnf angley() const {
		return (halnf) trigs::atan2(x, z);
	}
};