#pragma once

#include "common.h"
#include "trigonometry.h"

template <typename Type, const halni dim>
class vec {

	Type comp[dim];
	constexpr Type& get(alni i) const { return comp[i]; }

	public:

	vec() {}

	vec(const Type* src) {
		for (halni i = 0; i < dim; i++) {
			comp[i] = src[i];
		}
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

	// write
	vec& operator-() {
		for (halni i = 0; i < dim; i++) {
			get(i) = -get(i);
		}
		return *this_as_vec();
	}

	vec& operator+=(const vec& in) {
		for (halni i = 0; i < dim; i++) {
			get(i) += in.get(i);
		}
		return *this_as_vec();
	}
	vec& operator-=(const vec& in) {
		for (halni i = 0; i < dim; i++) {
			get(i) -= in.get(i);
		}
		return *this_as_vec();
	}
	vec& operator+=(const Type& val) {
		for (halni i = 0; i < dim; i++) {
			get(i) += val;
		}
		return *this_as_vec();
	}
	vec& operator-=(const Type& val) {
		for (halni i = 0; i < dim; i++) {
			get(i) -= val;
		}
		return *this_as_vec();
	}
	vec& operator*=(const Type& val) {
		for (halni i = 0; i < dim; i++) {
			get(i) *= val;
		}
		return *this_as_vec();
	}
	vec& operator/=(const Type& val) {
		for (halni i = 0; i < dim; i++) {
			get(i) /= val;
		}
		return *this_as_vec();
	}

	void assign(const Type& val) {
		for (halni i = 0; i < dim; i++) {
			get(i) = val;
		}
	}

	// create on stack
	vec operator+(const vec& in) const {
		vec out(*this_as_vec());
		out += in;
		return out;
	}
	vec operator-(const vec& in) const {
		vec out(*this_as_vec());
		out -= in;
		return out;
	}
	vec operator+(const Type& val) const {
		vec out(*this_as_vec());
		out += val;
		return out;
	}
	vec operator-(const Type& val) const {
		vec out(*this_as_vec());
		out -= val;
		return out;
	}
	vec operator*(const Type& val) const {
		vec out(*this_as_vec());
		out *= val;
		return out;
	}
	vec operator/(const Type& val) const {
		vec out(*this_as_vec());
		out /= val;
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
		return *this_as_vec();
	}

	vec unitv() {
		return vec(*this_as_vec()).normalize();
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