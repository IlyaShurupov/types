#pragma once

#include "trigonometry.h"

template <typename Type> class vec2;
using vec2f = vec2<halnf>;
using vec2i = vec2<halni>;

template <typename Type>
class vec2 {
	public:
	Type x;
	Type y;

	// Initialization
	template <typename TypeIn>
	vec2(TypeIn x, TypeIn y) {
		this->x = (Type) x;
		this->y = (Type) y;
	}
	template <typename TypeIn>
	vec2(TypeIn vec[2]) {
		x = (Type) vec[0];
		y = (Type) vec[1];
	}
	vec2() {
		x = y = 0;
	}
	template <typename TypeIn>
	vec2(TypeIn val) {
		x = y = (Type) val;
	}
	template <typename TypeIn>
	vec2(vec2<TypeIn>& vec) {
		x = (Type) vec.x;
		y = (Type) vec.y;
	}

	template <typename TypeIn>
	void assign(TypeIn x, TypeIn y) {
		this->x = (Type) x;
		this->y = (Type) y;
	}
	template <typename TypeIn>
	void assign(const vec2<TypeIn>& vec) {
		x = (Type) vec.x;
		y = (Type) vec.y;
	}
	template <typename TypeIn>
	void assign(TypeIn vec[2]) {
		x = (Type) vec[0];
		y = (Type) vec[1];
	}
	template <typename TypeIn>
	vec2<Type>& operator=(TypeIn val) {
		x = y = (Type) val;
		return *this;
	}

	// Conversion
	template <typename TypeIn>
	vec2<Type>& operator=(const vec2<TypeIn>& vec) {
		x = (Type) vec.x;
		y = (Type) vec.y;
		return *this;
	}

	//  create on stack
	vec2 operator+(const vec2& vec) const { return vec2(x + vec.x, y + vec.y); }
	vec2 operator-(const vec2& vec) const { return vec2(x - vec.x, y - vec.y); }
	vec2 operator+(Type val) const { return vec2(x + val, y + val); }
	vec2 operator-(Type val) const { return vec2(x - val, y - val); }
	vec2 operator*(Type val) const { return vec2(x * val, y * val); }
	vec2 operator/(Type val) const { return vec2(x / val, y / val); }

	//  write
	void operator-=(Type val) {
		x -= val;
		y -= val;
	}
	void operator+=(Type val) {
		x += val;
		y += val;
	}
	void operator-=(const vec2& vec) {
		x -= vec.x;
		y -= vec.y;
	}
	void operator+=(const vec2& vec) {
		x += vec.x;
		y += vec.y;
	}
	void operator*=(Type val) {
		x *= val;
		y *= val;
	}
	void operator/=(Type val) {
		x /= val;
		y /= val;
	}

	bool each_compre(const vec2<Type>& vec) const { return (x > vec.x && y > vec.y); }

	bool operator>(const vec2<Type>& vec) const { return lenpow2() > vec.lenpow2(); }
	bool operator<(const vec2<Type>& vec) const { return lenpow2() < vec.lenpow2(); }
	bool operator>=(const vec2<Type>& vec) const { return lenpow2() >= vec.lenpow2(); }
	bool operator<=(const vec2<Type>& vec) const { return lenpow2() <= vec.lenpow2(); }
	bool operator==(const vec2<Type>& vec) const { return (x == vec.x && y == vec.y); }

	Type& operator[](bool axes) { return (&x)[axes]; }
	const Type& operator[](bool axes) const { return (&x)[axes]; }

	// Vector Properties
	alnf operator*(const vec2& vec) const {
		return dot(vec);
	}
	alnf dot(const vec2& vec) const {
		return (x * vec.x + y * vec.y);
	}
	vec2 unitv() const {
		return vec2(*this / (Type) this->length());
	}
	void normalize() {
		*this /= (Type) this->length();
	}
	alnf length2() const {
		return (x * x + y * y);
	}
	alnf length() const {
		Type sqred = (Type) (x * x + y * y);
		return trigs::sqrt(sqred);
	}

	// Vector Transformation
	void Rot(float cosa, float sina) {
		Type tmp = x;
		x = x * cosa - y * sina;
		y = tmp * sina + y * cosa;
	}

	void clamp(const vec2<Type>& min, const vec2<Type>& max) {
		CLAMP(x, min.x, max.x);
		CLAMP(y, min.y, max.y);
	}
};