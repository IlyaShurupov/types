#pragma once

#include "trigonometry.h"

template <typename Type> class vec3;
using vec3f = vec3<alnf>;
using vec3i = vec3<alni>;

template <typename Type>
class vec3 {
public:
	union {
		Type x;
		Type r;
	};

	union {
		Type y;
		Type g;
	};

	union {
		Type z;
		Type b;
	};

public:

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
	void assign(Type vec[3]) {
		x = vec[0];
		y = vec[1];
		z = vec[2];
	}
	vec3& operator=(const vec3& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	//  create on stack
	vec3 operator+(const vec3& vec) const { return vec3(x + vec.x, y + vec.y, z + vec.z); }
	vec3 operator-(const vec3& vec) const { return vec3(x - vec.x, y - vec.y, z - vec.z); }
	vec3 operator+(Type val) const { return vec3(x + val, y + val, z + val); }
	vec3 operator-(Type val) const { return vec3(x - val, y - val, z - val); }
	vec3 operator*(Type val) const { return vec3(x * val, y * val, z * val); }
	vec3 operator/(Type val) const { return vec3(x / val, y / val, z / val);; }


	// compare
	bool operator>(const vec3<Type>& vec) const { return lenpow2() > vec.lenpow2(); }
	bool operator<(const vec3<Type>& vec) const { return lenpow2() < vec.lenpow2(); }
	bool operator>=(const vec3<Type>& vec) const { return lenpow2() >= vec.lenpow2(); }
	bool operator<=(const vec3<Type>& vec) const { return lenpow2() <= vec.lenpow2(); }
	bool operator==(const vec3<Type>& vec) const { return (x == vec.x && y == vec.y && z == vec.z); }
	
	//  write
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

	Type Dot(const vec3& vec) const { return (x * vec.x + y * vec.y + z * vec.z); }
	alnf Length() const { return sqrt((alnf)(x * x + y * y + z * z)); }
	Type lenpow2() const { return (x * x + y * y + z * z); }
	vec3 Dir() const { return *this / this->Length(); }
	void Normalize() { *this /= this->Length(); }
	vec3 Cross(const vec3& vec) const {
		Type x = this->y * vec.z - this->z * vec.y;
		Type y = this->z * vec.x - this->x * vec.z;
		Type z = this->x * vec.y - this->y * vec.x;
		return vec3(x, y, z);
	}

	// Vector Transformation

	void RotZ(alnf cosa, alnf sina) {
		Type tmp = x;
		x = (Type)(x * cosa - y * sina);
		y = (Type)(tmp * sina + y * cosa);
	}
	void RotY(float cosa, float sina) {
		Type tmp = x;
		x = (Type)(x * cosa - z * sina);
		z = (Type)(tmp * sina + z * cosa);
	}
	void RotX(float cosa, float sina) {
		Type tmp = y;
		y = (Type)(y * cosa - z * sina);
		z = (Type)(tmp * sina + z * cosa);
	}

};