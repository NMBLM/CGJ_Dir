#pragma once
#include <iostream>

namespace engine {

	struct vec2;
	struct vec3;
	struct vec4;

	/*
	struct vec2 
	{
		float x, y;
		// Constructor
		vec2(const float x,const float y);
		vec2(const vec2& v);

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
		vec3(const float k); // all values equal
		vec3(const float x, const float y, const float z);
		vec3(const float x, const float y, const float z, const float w); //vec4 to vec3 except all values, w = 1 means real world point w=0 means vector
		vec3(const vec3& v);
		vec3(const vec4& v);

		const float* data();


		// assignment, Comparison
		friend const bool operator==(const vec3& v1, const vec3& v2);
		friend const bool operator!=(const vec3& v1, const vec3& v2);

		vec3 operator+=(const vec3 v);
		vec3 operator-=(const vec3 v);
		vec3 operator*=(const float alpha);


		// Sum,sub, mult by scalar
		friend vec3 operator+(const vec3& v1, const vec3& v2);
		friend vec3 operator-(const vec3& v1, const vec3& v2);
		friend vec3 operator*(const vec3& v, const float alpha);
		friend vec3 operator*(const float alpha, const vec3& v);

		// cross,dot product
		const vec3 cross(const vec3& v1, const vec3& v2);
		const float dot(const vec3& v1, const vec3& v2);

		// input output
		friend std::ostream& operator<< (std::ostream& out, const vec3& v);
		friend std::istream& operator>> (std::istream& in, vec3& v);
	};

	
	struct vec4 {
		float x, y, z, w;
		/*
		// Constructor
		vec4(const float x, const float y,const float z,const float w);
		vec4(const vec3& v ,const float w = 0);
		// transform to diff dimension

		friend const bool operator==(const vec4& v1, const vec4& v2);
		friend const bool operator !=(const vec4& v1, const vec4& v2);

		// input output
		friend std::ostream & operator<< (std::ostream& out, const vec4& v);
		friend std::istream & operator>> (std::istream& in, vec4& v);
		*/
	};
	


}
