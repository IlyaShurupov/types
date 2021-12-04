#pragma once

#include <math.h>

class Vec3f
{
public:

	float x;
	float y;
	float z;

public:

	// Initialization

	Vec3f(float x, float y, float z);
	Vec3f(float vec[3]);
	Vec3f();
	Vec3f(const Vec3f& vec);
	void assign(float x, float y, float z);
  void assign(const Vec3f& vec);
	void assign(float vec[3]);
  Vec3f& operator=(const Vec3f& vec);

	// Fundamental create on stack

	Vec3f operator+(const Vec3f& vec);
  Vec3f operator-(const Vec3f& vec);
	Vec3f operator+(float val);
	Vec3f operator-(float val);
	Vec3f operator*(float val);
	Vec3f operator/(float val);
	// Fundamental write

	void operator -= (float val);
	void operator += (float val);
	void operator -= (const Vec3f& vec);
	void operator += (const Vec3f& vec);
	void operator *= (float val);
	void operator /= (float val);

	// Vector Properties

	float Dot(const Vec3f& vec);
	double Length();
	float LengthSqured();
	Vec3f Dir();
	void Normalize();
	Vec3f Cross(const Vec3f& vec);

	// Vector Transformation

	void RotZ(float cosa, float sina);
	void RotY(float cosa, float sina);
	void RotX(float cosa, float sina);

	~Vec3f();
};


inline Vec3f::Vec3f(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

inline Vec3f::Vec3f(float vec[3]) {
	x = vec[0];
	y = vec[1];
	z = vec[2];
}

inline Vec3f::Vec3f() {
	x = y = z = 0;
}

inline Vec3f::Vec3f(const Vec3f& vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

inline void Vec3f::operator -= (float val) {
	x -= val;
	y -= val;
	z -= val;
}

inline void Vec3f::operator += (float val) {
	x += val;
	y += val;
	z += val;
}

inline void Vec3f::operator -= (const Vec3f& vec) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
}

inline void Vec3f::operator += (const Vec3f& vec) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
}

inline void Vec3f::operator *= (float val) {
	x *= val;
	y *= val;
	z *= val;
}

inline void Vec3f::operator /= (float val) {
	x /= val;
	y /= val;
	z /= val;
}

inline Vec3f Vec3f::Cross(const Vec3f& vec) {
	float x = this->y * vec.z - this->z * vec.y;
	float y = this->z * vec.x - this->x * vec.z;
	float z = this->x * vec.y - this->y * vec.x;
  Vec3f newvec(x, y, z);
  return newvec;
}


inline void Vec3f::assign(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

inline void Vec3f::assign(const Vec3f& vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

inline void Vec3f::assign(float vec[3]) {
	x = vec[0];
	y = vec[1];
	z = vec[2];
}

inline Vec3f& Vec3f::operator=(const Vec3f& vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

// Fundamental create on stack

inline Vec3f Vec3f::operator+(const Vec3f& vec) {
  Vec3f newvec(x + vec.x, y + vec.y, z + vec.z);
  return newvec;
}

inline Vec3f Vec3f::operator-(const Vec3f& vec) {
  Vec3f newvec(x - vec.x, y - vec.y, z - vec.z);
  return newvec;
}

inline Vec3f Vec3f::operator+(float val) {
  Vec3f newvec(x + val, y + val, z + val);
  return newvec;
}

inline Vec3f Vec3f::operator-(float val) {
  Vec3f newvec(x - val, y - val, z - val);
  return newvec;
}

inline Vec3f Vec3f::operator*(float val) {
  Vec3f newvec(x * val, y * val, z * val);
  return newvec;
}

inline Vec3f Vec3f::operator/(float val) {
  Vec3f newvec(x / val, y / val, z / val);
  return newvec;
}

// Vector Properties

inline float Vec3f::Dot(const Vec3f& vec) {
	return (x * vec.x + y * vec.y + z * vec.z);
}

inline double Vec3f::Length() {
  return sqrt((float)(x * x + y * y + z * z));
}

inline float Vec3f::LengthSqured() {
	return (x * x + y * y + z * z);
}

inline Vec3f Vec3f::Dir() {
  Vec3f newrec1 = *this / (float)this->Length();
  Vec3f newrec(newrec1);
  return newrec;
}

inline void Vec3f::Normalize() {
	*this /= (float)this->Length();
}

// Vector Transformation

inline void Vec3f::RotZ(float cosa, float sina)
{
	float tmp = x;
	x = x * cosa - y * sina;
	y = tmp * sina + y * cosa;
}

inline void Vec3f::RotY(float cosa, float sina)
{
	float tmp = x;
	x = x * cosa - z * sina;
	z = tmp * sina + z * cosa;
}

inline void Vec3f::RotX(float cosa, float sina)
{
	float tmp = y;
	y = y * cosa - z * sina;
	z = tmp * sina + z * cosa;
}

inline Vec3f::~Vec3f() {

}