#pragma once

#include "vec.h"

template <typename Type, const halni nrow, const halni ncol>
class mat {
	typedef vec<Type, nrow> mvec;

	mvec col[ncol];

	public:

	mat() {}

	mat(const Type& val) {
		operator=(val);
	}

	mat(const mat& in) {
		operator=(in);
	}

	mvec& operator[](alni i) {
		assert(i < ncol&& i >= 0);
		return col[i];
	}

	const mvec& operator[](alni i) const {
		assert(i < ncol&& i >= 0);
		return col[i];
	}

	mat& operator=(const mat& in) {
		memcp(this, &in, sizeof(mat<Type, nrow, ncol>));
		return *this;
	}

	mat& operator=(const Type& val) {
		clear(val);
		return *this;
	}

	void clear(const Type& val) {
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				(*this)[i][j] = val;
			}
		}
	}

	void set_diagnal(const Type& val) {
		halni len = MIN(ncol, nrow);
		for (halni i = 0; i < len; i++) {
			(*this)[i][i] = val;
		}
	}

	mat operator-() {
		mat out;
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				out[i][j] = -(*this)[i][j];
			}
		}
		return out;
	}

	mat& operator+=(const mat& in) {
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				(*this)[i][j] += in[i][j];
			}
		}
		return *this;
	}

	mat& operator-=(const mat& in) {
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				(*this)[i][j] -= in[i][j];
			}
		}
		return *this;
	}

	mat& operator+=(const Type& val) {
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				(*this)[i][j] += val;
			}
		}
		return *this;
	}

	mat& operator-=(const Type& val) {
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				(*this)[i][j] -= val;
			}
		}
		return *this;
	}

	mat& operator/=(const Type& val) {
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				(*this)[i][j] /= val;
			}
		}
		return *this;
	}

	mat& operator*=(const Type& val) {
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				(*this)[i][j] *= val;
			}
		}
		return *this;
	}

	mat operator+(const mat& in) {
		mat out;
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				out[i][j] = (*this)[i][j] + in[i];
			}
		}
		return out;
	}
	mat operator-(const mat& in) {
		mat out;
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				out[i][j] = (*this)[i][j] - in[i];
			}
		}
		return out;
	}
	mat operator+(const Type& val) {
		mat out;
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				out[i][j] = (*this)[i][j] + val;
			}
		}
		return out;
	}
	mat operator-(const Type& val) {
		mat out;
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				out[i][j] = (*this)[i][j] - val;
			}
		}
		return out;
	}
	mat operator*(const Type& val) {
		mat out;
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				out[i][j] = (*this)[i][j] * val;
			}
		}
		return out;
	}
	mat operator/(const Type& val) {
		mat out;
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < nrow; j++) {
				out[i][j] = (*this)[i][j] / val;
			}
		}
		return out;
	}

	// Matrix Properties
	mvec transform(const mvec& in) const {
		assert(nrow == ncol);
		mvec out;
		for (halni i = 0; i < nrow; i++) {
			Type tmp = 0;
			for (halni j = 0; j < ncol; j++) {
				tmp += (*this)[i][j] * in[j];
			}
			out[i] = tmp;
		}
		return out;
	}

	mat transform(const mat& in) const {
		mat out;
		for (halni i = 0; i < ncol; i++) {
			out[i] = transform(in[i]);
		}
		return out;
	}

	void transpose() {
		assert(nrow == ncol);
		for (halni i = 0; i < ncol; i++) {
			for (halni j = 0; j < ncol; j++) {
				if (i != j) {
					SWAP((*this)[i][j], (*this)[j][i], const Type&);
				}
			}
		}
	}

	mat operator*(const mat& in) const {
		return transform(in);
	}

	mvec operator*(const mvec& in) const {
		return transform(in);
	}

	mat<Type, nrow - 1, nrow - 1> minor(halni p, halni q) {
		mat<Type, nrow - 1, nrow - 1> out;
		halni i = 0, j = 0;
		for (halni row = 0; row < nrow; row++) {
			for (halni col = 0; col < nrow; col++) {
				if (row != p && col != q) {
					out[i][j++] = (*this)[row][col];
					if (j == nrow - 1) {
						j = 0;
						i++;
					}
				}
			}
		}
		return out;
	}

	Type det() {
		assert(nrow == ncol);

		Type out = 0;

		if (nrow == 1) {
			return (*this)[0][0];
		}
		if (nrow == 2) {
			return ((*this)[0][0] * (*this)[1][1]) - ((*this)[1][0] * (*this)[0][1]);
		}

		halni sign = 1;
		for (int i = 0; i < nrow; i++) {
			out += sign * (*this)[0][i] * minor(0, i).det();
			sign = -sign;
		}
		return out;
	}

	mat cofactors() {
		assert(nrow == ncol);
		mat out;
		for (int i = 0; i < nrow; i++) {
			for (int j = 0; j < nrow; j++) {
				Type sign = (Type) ((i + j + 2) & 1 ? -1 : 1);
				out[i][j] = minor(j, i).det() * sign;
			}
		}
		return out;
	}

	mat inv() {
		Type detv = det();
		if (detv == 0) {
			return mat();
		}
		mat cof = cofactors();
		return cof /= detv;
	}
};

template <typename Type>
using mat2 = mat<Type, 2, 2>;
using mat2f = mat2<halnf>;

template <typename Type>
class mat<Type, 2, 2> {
	typedef vec<Type, 2> mvec;

	mvec i;
	mvec j;

	public:

	mat() {}

	mat(const Type& val) {
		operator=(val);
	}

	mat(const mvec& pi, const mvec& pj) {
		i.x = pi.x;
		i.y = pi.y;
		j.x = pj.x;
		j.y = pj.y;
	}

	mat(Type ix, Type iy, Type jx, Type jy) {
		i.x = ix;
		i.y = iy;
		j.x = jx;
		j.y = jy;
	}

	mat(const mat& in) {
		operator=(in);
	}

	mvec& operator[](alni idx) {
		assert(idx < 2 && idx >= 0);
		return (&i)[idx];
	}

	const mvec& operator[](alni idx) const {
		assert(idx < 2 && idx >= 0);
		return (&i)[idx];
	}

	mat& operator=(const mat& in) {
		memcp(this, &in, sizeof(mat2<Type>));
		return *this;
	}

	mat& operator=(const Type& val) {
		clear(val);
		return *this;
	}

	void clear(const Type& val) {
		i.x = val;
		i.y = val;
		j.x = val;
		j.y = val;
	}

	void set_diagnal(const Type& val) {
		i.x = val;
		j.y = val;
	}

	mat operator-() {
		return mat(-i.x, -i.y, -j.x, -j.y);
	}

	mat& operator+=(const mat& in) {
		i.x += in.i.x;
		i.y += in.i.y;
		j.x += in.j.x;
		j.y += in.j.y;
		return *this;
	}

	mat& operator-=(const mat& in) {
		i.x -= in.i.x;
		i.y -= in.i.y;
		j.x -= in.j.x;
		j.y -= in.j.y;
		return *this;
	}

	mat& operator+=(Type val) {
		i.x += val;
		i.y += val;
		j.x += val;
		j.y += val;
		return *this;
	}

	mat& operator-=(Type val) {
		i.x -= val;
		i.y -= val;
		j.x -= val;
		j.y -= val;
		return *this;
	}

	mat& operator/=(Type val) {
		i.x /= val;
		i.y /= val;
		j.x /= val;
		j.y /= val;
		return *this;
	}

	mat& operator*=(Type val) {
		i.x *= val;
		i.y *= val;
		j.x *= val;
		j.y *= val;
		return *this;
	}

	mat operator+(const mat& in) {
		mat out;
		out.i.x = i.x + in.i.x;
		out.i.y = i.y + in.i.y;
		out.j.x = j.x + in.j.x;
		out.j.y = j.y + in.j.y;
		return out;
	}
	mat operator-(const mat& in) {
		mat out;
		out.i.x = i.x - in.i.x;
		out.i.y = i.y - in.i.y;
		out.j.x = j.x - in.j.x;
		out.j.y = j.y - in.j.y;
		return out;
	}
	mat operator+(const Type& val) {
		mat out;
		out.i.x = i.x + val;
		out.i.y = i.y + val;
		out.j.x = j.x + val;
		out.j.y = j.y + val;
		return out;
	}
	mat operator-(const Type& val) {
		mat out;
		out.i.x = i.x - val;
		out.i.y = i.y - val;
		out.j.x = j.x - val;
		out.j.y = j.y - val;
		return out;
	}
	mat operator*(const Type& val) {
		mat out;
		out.i.x = i.x * val;
		out.i.y = i.y * val;
		out.j.x = j.x * val;
		out.j.y = j.y * val;
		return out;
	}
	mat operator/(const Type& val) {
		mat out;
		out.i.x = i.x / val;
		out.i.y = i.y / val;
		out.j.x = j.x / val;
		out.j.y = j.y / val;
		return out;
	}

	// Matrix Properties
	mvec transform(const mvec& in) const {
		return mvec(i.x * in.x + i.y * in.y, j.x * in.y + j.y * in.x);
	}

	mat transform(const mat& in) const {
		return mat(i.x * in.i.x + i.y * in.i.y, j.x * in.i.y + j.y * in.i.x,
			i.x * in.j.x + i.y * in.j.y, j.x * in.j.y + j.y * in.j.x);
	}

	mat operator*(const mat& in) {
		return transform(in);
	}

	mat& transpose() {
		SWAP(j.x, i.y, Type);
		return *this;
	}

	Type det() {
		return i.x * j.y - i.y * j.x;
	}

	mat cofactors() {
		return mat(j.y, -j.x, -i.y, i.x);
	}

	mat inv() {
		Type detv = det();
		if (detv == 0) {
			return mat();
		}
		mat cof = cofactors();
		return cof /= detv;
	}
};

template <typename Type>
using mat3 = mat<Type, 3, 3>;
using mat3f = mat3<halnf>;

template <typename Type>
class mat<Type, 3, 3> {
	typedef vec3<Type> vec;

	public:

	vec I;
	vec J;
	vec K;

	public:

	mat() {
		I.assign(1.f, 0.f, 0.f);
		J.assign(0.f, 1.f, 0.f);
		K.assign(0.f, 0.f, 1.f);
	}

	mat(const vec& i, const vec& j, const vec& k) {
		I = i;
		J = j;
		K = k;
	}
	mat(const mat& Matx) {
		this->I = Matx.I;
		this->J = Matx.J;
		this->K = Matx.K;
	}
	void assign(const vec& i, const vec& j, const vec& k) {
		I = i;
		J = j;
		K = k;
	}

	vec& operator[](alni i) {
		assert(i < 3 && i >= 0);
		return (&I)[i];
	}

	const vec& operator[](alni i) const {
		assert(i < 3 && i >= 0);
		return (&I)[i];
	}

	// create on stack
	mat operator+(const mat& in) { return mat(I + in.I, J + in.J, K + in.K); }
	mat operator-(const mat& in) { return mat(I - in.I, J - in.J, K - in.K); }
	mat operator+(Type val) { return mat(I + val, J + val, K + val); }
	mat operator-(Type val) { return mat(I - val, J - val, K - val); }
	mat operator*(Type val) { return mat(I * val, J * val, K * val); }
	mat operator/(Type val) { return mat(I / val, J / val, K / val); }

	// write
	mat& operator=(const mat& in) {
		I = in.I;
		J = in.J;
		K = in.K;
		return *this;
	}
	mat& operator+=(const mat& in) {
		I += in.I;
		J += in.J;
		K += in.K;
		return *this;
	}
	mat& operator-=(const mat& in) {
		I -= in.I;
		J -= in.J;
		K -= in.K;
		return *this;
	}
	mat& operator+=(Type val) {
		I += val;
		J += val;
		K += val;
		return *this;
	}
	mat& operator-=(Type val) {
		I -= val;
		J -= val;
		K -= val;
		return *this;
	}
	mat& operator*=(Type val) {
		I *= val;
		J *= val;
		K *= val;
		return *this;
	}
	mat& operator/=(Type val) {
		I /= val;
		J /= val;
		K /= val;
		return *this;
	}

	// Matrix transformation
	vec transform(const vec& in) {
		return vec(
			I.x * in.x + I.y * in.y + I.z * in.z,
			J.x * in.x + J.y * in.y + J.z * in.z,
			K.x * in.x + K.y * in.y + K.z * in.z);
	}

	mat transform(const mat& in) {
		return mat(transform(in.I), transform(in.J), transform(in.K));
	}

	vec operator*(const vec& in) {
		return transform(in);
	}

	mat operator*(const mat& in) {
		return transform(in);
	}

	mat<Type, 2, 2> minor(halni p, halni q) {
		mat<Type, 2, 2> out;
		switch (vec2i(p, q)) {
			case vec2i(0, 0):
				break;
		}
		return out;
	}

	mat& transpose() {
		SWAP(I.y, J.x, Type);
		SWAP(I.z, K.x, Type);
		SWAP(J.z, K.y, Type);
		return *this;
	}

	Type det() {
		return 0
			+ I.x * (J.y * K.z - J.z * K.y)
			- I.y * (J.x * K.z - J.z * K.x)
			+ I.z * (J.x * K.y - J.y * K.x);
	}

	mat cofactors() {
		return mat(
			vec(+(J.y * K.z - J.z * K.y), -(I.y * K.z - I.z * K.y), +(I.y * J.z - I.z * J.y)),
			vec(-(J.x * K.z - J.z * K.x), +(I.x * K.z - I.z * K.x), -(I.x * J.z - I.z * J.x)),
			vec(+(J.x * K.y - J.y * K.x), -(I.x * K.y - I.y * K.x), +(I.x * J.y - I.y * J.x))
		);
	}

	mat inv() {
		return cofactors() /= det();
	}

	mat rotx(alnf angle) {
		alnf cosa = (alnf) trigs::cos(angle);
		alnf sina = (alnf) trigs::sin(angle);
		return {
			{1, 0, 0},
			{0, cosa, -sina},
			{0, sina, cosa}
		};
	}
	mat roty(alnf angle) {
		alnf cosa = (alnf) trigs::cos(angle);
		alnf sina = (alnf) trigs::sin(angle);
		return {
			{cosa, 0, sina},
			{0, 1, 0},
			{-sina, 0, cosa}
		};
	}
	mat rotz(alnf angle) {
		alnf cosa = (alnf) trigs::cos(angle);
		alnf sina = (alnf) trigs::sin(angle);
		return {
			{cosa, -sina, 0},
			{sina, cosa, 0},
			{0, 0, 1}
		};
	}

	static mat rotmat(vec dir, alnf angle) {
		dir.normalize();

		mat out;
		alnf cosa = (alnf) trigs::cos(angle);
		alnf sina = (alnf) trigs::sin(angle);
		alnf tmp = 1 - cosa;

		out.I.x = (Type) (cosa + dir.x * dir.x * tmp);
		out.I.y = (Type) (dir.x * dir.y * tmp - dir.z * sina);
		out.I.z = (Type) (dir.x * dir.z * tmp + dir.y * sina);

		out.J.x = (Type) (dir.y * dir.x * tmp + dir.z * sina);
		out.J.y = (Type) (cosa + dir.y * dir.y * tmp);
		out.J.z = (Type) (dir.y * dir.z * tmp - dir.x * sina);

		out.K.x = (Type) (dir.z * dir.x * tmp - dir.y * sina);
		out.K.y = (Type) (dir.z * dir.y * tmp + dir.x * sina);
		out.K.z = (Type) (cosa + dir.z * dir.z * tmp);

		return out;
	}
};

template <typename Type >
using mat4 =  mat<Type, 4, 4>;

using mat4f = mat4<halnf>;
using mat4i = mat4<halni>;

template <typename Type >
class mat < Type, 0, 0 > {
	typedef vec<Type, 1> mvec;

	mvec dummy;

	public:

	mvec& operator[](alni i) {
		return dummy;
	}

	const mvec& operator[](alni i) const {
		return dummy;
	}

	Type det() {
		return Type();
	}
};