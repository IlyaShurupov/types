

#include "typetraits.h"

#include "vec.h"

#include "mat.h"

#include <stdio.h>

void mattest() {
	mat<halnf, 4, 4> tmat(0);

	tmat.set_diagnal(2);

	auto tmat_inv = tmat.inv();
	auto idenity_mat = tmat_inv * tmat;

	typedef compiler::TypeTraits<const mat<halnf, 4, 4>> mat4f_traits;

	printf("%i \n", mat4f_traits::isPointer);
	printf("%i \n", mat4f_traits::isConst);
	printf("%i \n", mat4f_traits::isVolatile);
}


void vec_test() {
	vec<alnf, 2> vec2;
	vec<alnf, 2> vec22;
	
	//vec2.x = 4;
	//vec2.y = 4;

	vec22 = vec2;

	bool f = vec22 == vec2;

	printf("%f \n", vec2.length2());
	printf("%f \n", (vec2 / 2).length2());

	vec<alnf, 4> vec4;
	vec4[0] = 1;
	vec4[1] = 1;
	vec4[2] = 1;
	vec4[3] = 1;
	printf("%f \n", vec4.length2());
}

int main() {
	vec_test();
}