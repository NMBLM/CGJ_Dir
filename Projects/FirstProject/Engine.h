#pragma once
#include <iostream>
namespace engine {
	/*
	struct vec2 
	{
		float x, y;
		// Constructor
		vec2(float x, float y);

		// transform to diff dimension

		// assignment, Comparison
		void operator=(const vec2& v);

		friend const bool operator==(const vec2& v1, const vec2& v2);


		// Sum,sub, mult by scalar
		friend vec2 operator+(const vec2& v1, const vec2& v2);
		friend vec2 operator-(const vec2& v1, const vec2& v2);
		friend vec2 operator*(const vec2& v, const float alpha);
		friend vec2 operator*(const float alpha, const vec2& v);


		// input output
		friend std::ostream & operator<< (std::ostream &out, const vec2 &v);
		friend std::istream & operator>> (std::istream &in, vec2 &v);
	};
	*/


	struct vec3 {
		float x, y, z;

		// Constructor
		vec3(float x, float y, float z);

		// transform to diff dimension

		// assignment, Comparison
		void operator=(const vec3& v);

		friend const bool operator==(const vec3& v1, const vec3& v2);


		// Sum,sub, mult by scalar
		friend vec3 operator+(const vec3& v1, const vec3& v2);
		friend vec3 operator-(const vec3& v1, const vec3& v2);
		friend vec3 operator*(const vec3& v, const float alpha);
		friend vec3 operator*(const float alpha, const vec3& v);

		// cross,dot product
		const vec3 cross(const vec3& v1, vec3& v2);
		const float dot(const vec3& v1, vec3& v2);

		// input output
		friend std::ostream & operator<< (std::ostream& out, const vec3& v);
		friend std::istream & operator>> (std::istream& in, vec3& v);
	};

	/*
	struct vec4 {
		float x, y, z, w;
		
		// Constructor
		vec4(float x, float y, float z);

		// transform to diff dimension

		// assignment, Comparison
		void operator=(const vec4& v);

		friend const bool operator==(const vec4& v1, const vec4& v2);


		// Sum,sub, mult by scalar
		friend vec4 operator+(const vec4& v1, const vec4& v2);
		friend vec4 operator-(const vec4& v1, const vec4& v2);
		friend vec4 operator*(const vec4& v, const float alpha);
		friend vec4 operator*(const float alpha, const vec4& v);

		// cross,dot product
		const vec4 cross(const vec4& v1, vec4& v2);
		const float dot(const vec4& v1, vec4& v2);

		// input output
		friend std::ostream & operator<< (std::ostream& out, const vec4& v);
		friend std::istream & operator>> (std::istream& in, vec4& v);
	};
	*/
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
		float mat[9];

		//constructor

		mat3(float a, float b, float c, // First Row
			float d, float e, float f,	// Second Row
			float g, float h, float i); // Third Row

		// transform to diff dimension


		//assignment, comparison

		//input/output

		//sum, subtraction 

		//multiplication (by another matrix, a scalar, or a vector) 

		//transpose, determinant and inverse (except for 4x4 matrices)
	};
	/*
	struct mat4 {
		float mat[16];

		//constructor

		//assignment, comparison

		//input/output

		//sum, subtraction 

		//multiplication (by another matrix, a scalar, or a vector) 

		//transpose, determinant and inverse (except for 4x4 matrices)
	};
	*/

	struct MatrixFactory {

		// identity
		const mat3 Matrice3Identity();

		// scale ( x, y, z);

		// translation (x,y,z) revisit this

		// rotation ( angle )

		// dual is for cross Product

		// dual ( vec2 )

		// dual ( vec3 )

		// dual ( vec4 )

		//Shear/Sheer?
	};


}
