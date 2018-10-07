#pragma once
#include "vector.h"

namespace engine {

	struct mat2;
	struct mat3;
	struct mat4;
	/*
	struct mat2 {
		float mat[4];

		//constructor

		// transform to diff dimension


		//assignment, comparison

		//input/output

		//sum, subtraction

		//multiplication (by another matrix, a scalar, or a vector)

		//transpose, determinant and inverse (except for 4x4 matrices)


	};
	*/
	struct mat3 {
		float mat[9] = {};
		/*
		|	0	1	2	|
		|	3	4	5	|
		|	6	7	8	|
		*/
		//constructor
		mat3(const float k);

		mat3(const float m11, const  float m12, const  float m13,	// First Row
			 const float m21, const  float m22, const  float m23,	// Second Row
			 const float m31, const  float m32, const  float m33);	// Third Row
		mat3(const mat3& m);
		mat3(const mat4& m);


	   //assignment, comparison

	   //input/output

	   //sum, subtraction 

	   //multiplication (by another matrix, a scalar, or a vector) 
		friend mat3 operator* (const mat3& m1, const mat3& m2);
		friend mat3 operator* (const mat3& m, const float alpha);
		friend mat3 operator* (const float alpha, const mat3& m);
		friend vec3 operator* (const mat3& m, const vec3& v);
	   //transpose, determinant and inverse (except for 4x4 matrices)

		friend std::ostream& operator<< (std::ostream& out, const mat3& m);
		friend std::istream& operator>> (std::istream& in, mat3& m);
	};
	
	struct mat4 {
		float mat[16];
		/*
		|	0	1	2	3	|
		|	4	5	6	7	|
		|	8	9	10	11	|
		|	12	13	14	15	|
		*/
		/*
		//constructor
		mat4(const float k);

		mat4(const float m11, const  float m12, const  float m13, const  float m14,	 // First  Row
			 const float m21, const  float m22, const  float m23, const  float m24,	 // Second Row
			 const float m31, const  float m32, const  float m33, const  float m34,  // Third  Row
			 const float m41, const  float m42, const  float m43, const  float m44); // Fourth Row	

		mat4(const mat3& m, const float w=0);
		mat4(const mat4& m);
		//assignment, comparison

		//input/output

		//sum, subtraction

		//multiplication (by another matrix, a scalar, or a vector)

		//transpose, determinant and inverse (except for 4x4 matrices)
		*/
	};
	

	struct MatrixFactory {

		// identity
		static const mat2 createIdentityMatrix2();
		static const mat3 createIdentityMatrix3();
		static const mat4 createIdentityMatrix4();


		// scale ( x, y, z);

		// translation (x,y,z) revisit this

		// rotation ( angle )

		// dual is for cross Product
		static const mat3 createDualMatrix(const vec3 v);

		//Shear/Sheer?
	};

}