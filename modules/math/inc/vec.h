#pragma once

#include "common.h"
#include "trigonometry.h"

template <typename Type, const halni dim>
class vec {

	Type comp[dim];
	constexpr const Type& get(alni i) const { return comp[i]; }
	constexpr Type& get(alni i) { return comp[i]; }

	public:

	vec() {}

	vec(Type val) {
		assign(val);
	}

	vec(Type val1, Type val2, Type val3, Type val4) {
		static_assert(dim == 4);
		comp[0] = val1;
		comp[1] = val2;
		comp[2] = val3;
		comp[3] = val4;
	}

	vec(Type val1, Type val2, Type val3, Type val4, Type val5) {
		static_assert(dim == 5);
		comp[0] = val1;
		comp[1] = val2;
		comp[2] = val3;
		comp[3] = val4;
		comp[4] = val5;
	}

	vec(Type val1, Type val2, Type val3, Type val4, Type val5, Type val6) {
		static_assert(dim == 6);
		comp[0] = val1;
		comp[1] = val2;
		comp[2] = val3;
		comp[3] = val4;
		comp[4] = val5;
		comp[5] = val6;
	}

	vec(const vec& in) {
		memcp(comp, in.comp, sizeof(Type) * dim);
	}

	Type& operator[](alni i) {
		assert(i < dim&& i >= 0);
		return comp[i];
	}

	const Type& operator[](alni i) const {
		assert(i < dim&& i >= 0);
		return comp[i];
	}

	vec operator-() {
		vec out;
		for (halni i = 0; i < dim; i++) {
			out.get(i) = -get(i);
		}
		return out;
	}

	// write

	vec& operator+=(const vec& val) {
		for (halni i = 0; i < dim; i++) {
			get(i) += val[i];
		}
		return *this;
	}
	vec& operator-=(const vec& val) {
		for (halni i = 0; i < dim; i++) {
			get(i) -= val[i];
		}
		return *this;
	}
	vec& operator+=(Type val) {
		for (halni i = 0; i < dim; i++) {
			get(i) += val;
		}
		return *this;
	}
	vec& operator-=(Type val) {
		for (halni i = 0; i < dim; i++) {
			get(i) -= val;
		}
		return *this;
	}
	vec& operator*=(Type val) {
		for (halni i = 0; i < dim; i++) {
			get(i) *= val;
		}
		return *this;
	}
	vec& operator/=(Type val) {
		for (halni i = 0; i < dim; i++) {
			get(i) /= val;
		}
		return *this;
	}

	void assign(Type val) {
		for (halni i = 0; i < dim; i++) {
			get(i) = val;
		}
	}

	// create on stack
	vec operator+(const vec& in) const {
		vec out;
		for (halni i = 0; i < dim; i++) {
			out[i] = in[i] + get(i);
		}
		return out;
	}
	vec operator-(const vec& in) const {
		vec out;
		for (halni i = 0; i < dim; i++) {
			out[i] = in[i] - get(i);
		}
		return out;
	}
	vec operator+(Type val) const {
		vec out;
		for (halni i = 0; i < dim; i++) {
			out[i] = get(i) + val;
		}
		return out;
	}
	vec operator-(Type val) const {
		vec out;
		for (halni i = 0; i < dim; i++) {
			out[i] = get(i) - val;
		}
		return out;
	}
	vec operator*(Type val) const {
		vec out;
		for (halni i = 0; i < dim; i++) {
			out[i] = get(i) * val;
		}
		return out;
	}
	vec operator/(Type val) const {
		vec out;
		for (halni i = 0; i < dim; i++) {
			out[i] = get(i) / val;
		}
		return out;
	}

	// Vector Properties
	Type dot(const vec& in) const {
		Type out = 0;
		for (halni i = 0; i < dim; i++) {
			out += get(i) * in.get(i);
		}
		return out;
	}

	Type operator*(const vec& in) const {
		return dot(in);
	}

	alnf length2() const {
		alnf summ = 0;
		for (halni i = 0; i < dim; i++) {
			Type val = get(i);
			summ += val * val;
		}
		return summ;
	}

	alnf length() const {
		return trigs::sqrt(length2());
	}

	vec& normalize() {
		operator/=((Type) length());
		return *this;
	}

	vec unitv() {
		return vec(*this).normalize();
	}

	// Comparisons
	bool operator>(const vec& vec) const { return this->length2() > vec.length2(); }
	bool operator<(const vec& vec) const { return this->length2() < vec.length2(); }
	bool operator>=(const vec& vec) const { return this->length2() >= vec.length2(); }
	bool operator<=(const vec& vec) const { return this->length2() <= vec.length2(); }
	bool operator==(const vec& vec) const {
		for (halni i = 0; i < dim; i++) {
			if (get(i) != vec.get(i)) {
				return false;
			}
		}
		return true;
	}
	bool operator!=(const vec& vec) const {
		return !operator==(vec);
	}
};

template <typename Type>
using vec2 = vec<Type, 2>;

using vec2f = vec<halnf, 2>;
using vec2i = vec<halni, 2>;

template <typename Type>
class vec<Type, 2> {
	typedef vec2<Type> vec2;

	public:
	Type x;
	Type y;

	// Initialization
	template <typename TypeIn>
	vec(TypeIn x, TypeIn y) {
		this->x = (Type) x;
		this->y = (Type) y;
	}
	template <typename TypeIn>
	vec(TypeIn vec[2]) {
		x = (Type) vec[0];
		y = (Type) vec[1];
	}
	vec() {
		x = y = 0;
	}
	template <typename TypeIn>
	vec(TypeIn val) {
		x = y = (Type) val;
	}
	template <typename TypeIn>
	vec(vec<TypeIn, 2>& vec) {
		x = (Type) vec.x;
		y = (Type) vec.y;
	}

	template <typename TypeIn>
	void assign(TypeIn x, TypeIn y) {
		this->x = (Type) x;
		this->y = (Type) y;
	}
	template <typename TypeIn>
	void assign(const vec<TypeIn, 2>& vec) {
		x = (Type) vec.x;
		y = (Type) vec.y;
	}
	template <typename TypeIn>
	void assign(TypeIn vec[2]) {
		x = (Type) vec[0];
		y = (Type) vec[1];
	}
	template <typename TypeIn>
	vec& operator=(TypeIn val) {
		x = y = (Type) val;
		return *this;
	}

	// Conversion
	template <typename TypeIn>
	vec& operator=(const vec<TypeIn, 2>& vec) {
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

	bool each_compre(const vec& vec) const { return (x > vec.x && y > vec.y); }

	bool operator>(const vec& vec) const { return length2() > vec.length2(); }
	bool operator<(const vec& vec) const { return length2() < vec.length2(); }
	bool operator>=(const vec& vec) const { return length2() >= vec.length2(); }
	bool operator<=(const vec& vec) const { return length2() <= vec.length2(); }
	bool operator==(const vec& vec) const { return (x == vec.x && y == vec.y); }

	Type& operator[](bool axes) { return (&x)[axes]; }
	const Type& operator[](bool axes) const { return (&x)[axes]; }

	// Vector Properties
	alnf operator*(const vec& vec) const {
		return dot(vec);
	}
	alnf dot(const vec& vec) const {
		return (x * vec.x + y * vec.y);
	}
	vec unitv() const {
		return vec(*this / (Type) this->length());
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

	void clamp(const vec& min, const vec& max) {
		CLAMP(x, min.x, max.x);
		CLAMP(y, min.y, max.y);
	}
};

template <typename Type>
using vec3 = vec<Type, 3>;

using vec3f = vec3<halnf>;
using vec3i = vec3<halni>;

template <typename Type>
class vec<Type, 3> {

	public:

	Type x;
	Type y;
	Type z;

	// Initialization
	vec() {}

	vec(const vec<Type, 4>& in) {
		x = in[0];
		y = in[1];
		z = in[2];
	}

	vec(Type x, Type y, Type z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	vec(Type vec[3]) {
		x = vec[0];
		y = vec[1];
		z = vec[2];
	}
	vec(Type x) {
		assign(x);
	}
	vec(const vec& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	void assign(Type x, Type y, Type z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void assign(const vec& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	void assign(Type val) {
		x = val;
		y = val;
		z = val;
	}
	vec& randf() {
		x = (Type)::randf();
		y = (Type)::randf();
		z = (Type)::randf();
		return *this;
	}
	vec& operator=(const vec& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	vec& operator=(const Type* in) {
		x = in[0];
		y = in[1];
		z = in[2];
		return *this;
	}

	Type& operator[](alni i) {
		assert(i < 3 && i >= 0);
		return (&x)[i];
	}

	const Type& operator[](alni i) const {
		assert(i < 3 && i >= 0);
		return (&x)[i];
	}

	//  create on stack
	vec operator+(const vec& in) const {
		return vec(x + in.x, y + in.y, z + in.z);
	}
	vec operator-(const vec& in) const {
		return vec(x - in.x, y - in.y, z - in.z);
	}
	vec operator+(Type val) const {
		return vec(x + val, y + val, z + val);
	}
	vec operator-(Type val) const {
		return vec(x - val, y - val, z - val);
	}
	vec operator*(Type val) const {
		return vec(x * val, y * val, z * val);
	}
	vec operator/(Type val) const {
		return vec(x / val, y / val, z / val);;
	}


// compare
	bool operator>(const vec& vec) const {
		return length2() > vec.length2();
	}
	bool operator<(const vec& vec) const {
		return length2() < vec.length2();
	}
	bool operator>=(const vec& vec) const {
		return length2() >= vec.length2();
	}
	bool operator<=(const vec& vec) const {
		return length2() <= vec.length2();
	}
	bool operator==(const vec& vec) const {
		return (x == vec.x && y == vec.y && z == vec.z);
	}

//  write
	vec operator-() {
		return vec(-x, -y, -z);
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
	void operator -= (const vec& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}
	void operator += (const vec& vec) {
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

	vec& random() {
		x = randf();
		y = randf();
		z = randf();
		return *this;
	}

	vec& randomunit() {
		random();
		normalize();
		return *this;
	}

	// Vector Properties

	Type dot(const vec& vec) const {
		return (x * vec.x + y * vec.y + z * vec.z);
	}
	alnf length() const {
		return trigs::sqrt((halnf) (x * x + y * y + z * z));
	}
	Type length2() const {
		return (x * x + y * y + z * z);
	}
	vec unitv() const {
		return *this / (Type) this->length();
	}
	vec& normalize() {
		*this /= (Type) this->length();
		return *this;
	}
	vec cross(const vec& in) const {
		return vec(
			y * in.z - z * in.y,
			z * in.x - x * in.z,
			x * in.y - y * in.x
		);
	}
	vec operator*(const vec& in) const {
		return cross(in);
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

	halnf anglex() const {
		return (halnf) trigs::atan2(y, z);
	}

	halnf angley() const {
		return (halnf) trigs::atan2(x, z);
	}
};

template <typename Type>
using vec4 = vec<Type, 4>;

using vec4f = vec4<halnf>;
using vec4i = vec4<halni>;