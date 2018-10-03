#include "matrix.h"
#include "vector.h"

namespace engine {
	
	// mat3 constructor
	mat3::mat3(const float k) // diagonal is same value
	{
		mat[0] = mat[4] = mat[8] = k;
	}
	mat3::mat3(const float m11, const float m12, const float m13,
		const float m21, const float m22, const float m23,
		const float m31, const float m32, const float m33) {
		mat[0] = m11;	mat[1] = m12;	mat[2] = m13;
		mat[3] = m21;	mat[4] = m22;	mat[5] = m23;
		mat[6] = m31;	mat[7] = m32;	mat[8] = m33;
	}
	mat3::mat3(const mat3& m) {
		// This seems stupid
		mat[0] = m.mat[0];	mat[1] = m.mat[1];	mat[2] = m.mat[2];
		mat[3] = m.mat[3];	mat[4] = m.mat[4];	mat[5] = m.mat[5];
		mat[6] = m.mat[6];	mat[7] = m.mat[7];	mat[8] = m.mat[8];

	}
	mat3::mat3(const mat4& m) {
		//TODO
	}


	vec3 operator*(const mat3 & m, const vec3 & v)
	{	
		vec3 ret = vec3(0);
		ret.x = m.mat[0] * v.x + m.mat[1] * v.y + m.mat[2] * v.z;
		ret.y = m.mat[3] * v.x + m.mat[4] * v.y + m.mat[5] * v.z;
		ret.z = m.mat[6] * v.x + m.mat[7] * v.y + m.mat[8] * v.z;
		return ret;
	}


	const mat3 MatrixFactory::createIdentityMatrix3() {
		return mat3(1);
	}
	const mat3 MatrixFactory::createDualMatrix(const vec3 v) {
		return mat3(0, -v.z, v.y,
					v.z, 0, -v.x,
					-v.y, v.x, 0);
	}

	


}
