#include "matrix.h"
#include "vector.h"

namespace engine {
	// mat3 constructor
	mat3::mat3(const float m11, const float m12, const float m13,
		const float m21, const float m22, const float m23,
		const float m31, const float m32, const float m33) {
		mat[0] = m11;	mat[1] = m12;	mat[2] = m13;
		mat[3] = m21;	mat[4] = m22;	mat[5] = m23;
		mat[6] = m31;	mat[7] = m32;	mat[8] = m33;
	}

	const mat3 MatrixFactory::createDualMatrice(const vec3 v) {
		return mat3(   0, -v.z,  v.y,
					 v.z,    0, -v.x,
					-v.y,  v.x,    0 );
	}

	vec3 operator*(const mat3 & m, const vec3 & v)
	{	
		vec3 ret = vec3(0);
		ret.x = m.mat[0] * v.x + m.mat[1] * v.y + m.mat[2] * v.z;
		ret.y = m.mat[3] * v.x + m.mat[4] * v.y + m.mat[5] * v.z;
		ret.z = m.mat[6] * v.x + m.mat[7] * v.y + m.mat[8] * v.z;
		return ret;
	}

	

}
