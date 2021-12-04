#pragma once

#include "Vec3.h"

class Mat3f
{
public:
	Vec3f I;
	Vec3f J;
	Vec3f K;

public:

	Mat3f();
	~Mat3f();
	Mat3f(const Vec3f& i, const Vec3f& j, const Vec3f& k);
	Mat3f(const Mat3f& Matx);
	void Assign(const Vec3f& i, const Vec3f& j, const Vec3f& k);

	// Fundamental create on stack

	Mat3f operator+(const Mat3f& Matx);
	Mat3f operator-(const Mat3f& Matx);
	Mat3f operator+(float val);
	Mat3f operator-(float val);
	Mat3f operator*(float val);
	Mat3f operator/(float val);

	// Fundamental write

	void operator=(const Mat3f& Matx);
	void operator+=(const Mat3f& Matx);
	void operator-=(const Mat3f& Matx);
	void operator+=(float val);
	void operator-=(float val);
	void operator*=(float val);
	void operator/=(float val);

	// Matrix transformation

	Vec3f& Transform(Vec3f& vec);
	Mat3f& Transform(Mat3f& Matx);
};

inline Mat3f::Mat3f() {
	I.assign(1.f, 0.f, 0.f);
	J.assign(0.f, 1.f, 0.f);
	K.assign(0.f, 0.f, 1.f);
}

inline Mat3f::~Mat3f() {
}

inline Mat3f::Mat3f(const Vec3f& i, const Vec3f& j, const Vec3f& k) {
	I = i;
	J = j;
	K = k;
}

inline Mat3f::Mat3f(const Mat3f& Matx) {
	this->I = Matx.I;
	this->J = Matx.J;
	this->K = Matx.K;
}

inline void Mat3f::Assign(const Vec3f& i, const Vec3f& j, const Vec3f& k) {
	I = i;
	J = j;
	K = k;
}

// Fundamental create on stack

inline Mat3f Mat3f::operator+(const Mat3f& Matx) {
	return Mat3f(this->I + Matx.I, this->J + Matx.J, this->K + Matx.K);
}

inline Mat3f Mat3f::operator-(const Mat3f& Matx) {
	return Mat3f(this->I - Matx.I, this->J - Matx.J, this->K - Matx.K);
}

inline Mat3f Mat3f::operator+(float val) {
	return Mat3f(this->I + val, this->J + val, this->K + val);
}

inline Mat3f Mat3f::operator-(float val) {
	return Mat3f(this->I - val, this->J - val, this->K - val);
}

inline Mat3f Mat3f::operator*(float val) {
	return Mat3f(this->I * val, this->J * val, this->K * val);
}

inline Mat3f Mat3f::operator/(float val) {
	return Mat3f(this->I / val, this->J / val, this->K / val);
}

// Fundamental write

inline void Mat3f::operator=(const Mat3f& Matx) {
	this->I = Matx.I;
	this->J = Matx.J;
	this->K = Matx.K;
}

inline void Mat3f::operator+=(const Mat3f& Matx) {
	this->I += Matx.I;
	this->J += Matx.J;
	this->K += Matx.K;
}

inline void Mat3f::operator-=(const Mat3f& Matx) {
	this->I -= Matx.I;
	this->J -= Matx.J;
	this->K -= Matx.K;
}

inline void Mat3f::operator+=(float val) {
	this->I += val;
	this->J += val;
	this->K += val;
}

inline void Mat3f::operator-=(float val) {
	this->I -= val;
	this->J -= val;
	this->K -= val;
}

inline void Mat3f::operator*=(float val) {
	this->I *= val;
	this->J *= val;
	this->K *= val;
}

inline void Mat3f::operator/=(float val) {
	this->I /= val;
	this->J /= val;
	this->K /= val;
}

// Matrix transformation

inline Vec3f& Mat3f::Transform(Vec3f& vec) {
	float x = I.x * vec.x + J.x * vec.y + K.x * vec.z;
	float y = I.y * vec.x + J.y * vec.y + K.y * vec.z;
	float z = I.z * vec.x + J.z * vec.y + K.z * vec.z;
	vec.assign(x, y, z);
	return vec;
}

inline Mat3f& Mat3f::Transform(Mat3f& Matx) {
	Matx.Assign(this->Transform(Matx.I), this->Transform(Matx.J), this->Transform(Matx.K));
	return Matx;
}


inline void RotArbAxis(Mat3f& Mat, const Vec3f& Dir, float angle) {

	float cosa = (float)cos(angle);
	float sina = (float)sin(angle);
	float tmp = 1 - cosa;

	Mat.I.x = cosa + Dir.x * Dir.x* tmp;
	Mat.I.y = Dir.y * Dir.x * tmp + Dir.z * sina;
	Mat.I.z = Dir.z * Dir.x * tmp - Dir.y * sina;

	Mat.J.x = Dir.x * Dir.y * tmp + Dir.z * sina;
	Mat.J.y = cosa + Dir.y * Dir.y * tmp;
	Mat.J.z = Dir.z * Dir.y * tmp + Dir.x * sina;

	Mat.K.x = Dir.x * Dir.z * tmp + Dir.y * sina;
	Mat.K.y = Dir.y * Dir.z * tmp + Dir.x * sina;
	Mat.K.z = cosa + Dir.z * Dir.z * tmp;
}
