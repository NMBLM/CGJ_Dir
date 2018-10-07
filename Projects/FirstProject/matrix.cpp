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

		mat[0] = m.mat[0];	mat[1] = m.mat[1];	mat[2] = m.mat[2];
		mat[3] = m.mat[4];	mat[4] = m.mat[5];	mat[5] = m.mat[6];
		mat[6] = m.mat[8];	mat[7] = m.mat[9];	mat[8] = m.mat[10];
	}


	mat3 operator*(const mat3 & m1, const mat3 & m2)
	{
		/*
		|	0	1	2	| 		|	a	b	c	|		|	0*a+1*d+2*g		0*b+1*e+2*h		0*c+1*f+2*i		|	
		|	3	4	5	|	*	|	d	e	f	|	=	|	3*a+4*d+5*g		3*b+4*e+5*h		3*c+4*f+5*i		|
		|	6	7	8	|		|	g	h	i	|		|	6*a+7*d+8*g		6*b+7*e+8*h		6*c+7*f+8*i		|
		*/
		return mat3(m1.mat[0] * m2.mat[0] + m1.mat[1] * m2.mat[3] + m1.mat[2] * m2.mat[6],	//m11
					m1.mat[0] * m2.mat[1] + m1.mat[1] * m2.mat[4] + m1.mat[2] * m2.mat[7],	//m12
					m1.mat[0] * m2.mat[2] + m1.mat[1] * m2.mat[5] + m1.mat[2] * m2.mat[8],	//m13
					m1.mat[3] * m2.mat[0] + m1.mat[4] * m2.mat[3] + m1.mat[5] * m2.mat[6],	//m21
					m1.mat[3] * m2.mat[1] + m1.mat[4] * m2.mat[4] + m1.mat[5] * m2.mat[7],	//m22 
					m1.mat[3] * m2.mat[2] + m1.mat[4] * m2.mat[5] + m1.mat[5] * m2.mat[8],	//m23
					m1.mat[6] * m2.mat[0] + m1.mat[7] * m2.mat[3] + m1.mat[8] * m2.mat[6],	//m31
					m1.mat[6] * m2.mat[1] + m1.mat[7] * m2.mat[4] + m1.mat[8] * m2.mat[7],	//m32 
					m1.mat[6] * m2.mat[2] + m1.mat[7] * m2.mat[5] + m1.mat[8] * m2.mat[8]);	//m33
		}

	mat3 operator*(const mat3 & m, const float alpha)
	{
		return mat3(m.mat[0] * alpha, m.mat[1] * alpha, m.mat[2] * alpha,
					m.mat[3] * alpha, m.mat[4] * alpha, m.mat[5] * alpha,
					m.mat[6] * alpha, m.mat[7] * alpha, m.mat[8] * alpha);
	}

	mat3 operator*(const float alpha, const mat3 & m)
	{
		return mat3(m.mat[0] * alpha, m.mat[1] * alpha, m.mat[2] * alpha,
			m.mat[3] * alpha, m.mat[4] * alpha, m.mat[5] * alpha,
			m.mat[6] * alpha, m.mat[7] * alpha, m.mat[8] * alpha);
	}

	vec3 operator*(const mat3 & m, const vec3 & v)
	{	
		vec3 ret = vec3(0);
		ret.x = m.mat[0] * v.x + m.mat[1] * v.y + m.mat[2] * v.z;
		ret.y = m.mat[3] * v.x + m.mat[4] * v.y + m.mat[5] * v.z;
		ret.z = m.mat[6] * v.x + m.mat[7] * v.y + m.mat[8] * v.z;
		return ret;
	}

	std::ostream & operator<<(std::ostream & out, const mat3 & m)
	{
		out << "| " << m.mat[0] << "  " << m.mat[1] << "  " << m.mat[2] << " |" << std::endl;
		out << "| " << m.mat[3] << "  " << m.mat[4] << "  " << m.mat[5] << " |" << std::endl;
		out << "| " << m.mat[6] << "  " << m.mat[7] << "  " << m.mat[8] << " |" << std::endl;

		return out;
	}

	std::istream & operator>>(std::istream & in, mat3 & m)
	{
		in  >> m.mat[0] >> m.mat[1] >> m.mat[2] 
			>> m.mat[3] >> m.mat[4] >> m.mat[5] 
			>> m.mat[6] >> m.mat[7] >> m.mat[8];

		return in;
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
