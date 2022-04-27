

#include "typetraits.h"

#include "vec.h"

#include "mat.h"

#include <stdio.h>

void mat_test() {
	mat2<halnf> tmat2;
	tmat2.randf();
	auto tmp2 = tmat2.inv();
	auto idenity_mat2 = tmp2 * tmat2;

	mat3<halnf> tmat3;
	tmat3.randf();
	auto tmp3 = tmat3.inv();
	auto idenity_mat3 = tmp3 * tmat3;

	mat4<halnf> tmat4;
	tmat4.randf();
	auto tmp4 = tmat4.inv();
	auto idenity_mat4 = tmp4 * tmat4;
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
	mat_test();
}