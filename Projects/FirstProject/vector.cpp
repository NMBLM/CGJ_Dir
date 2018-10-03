#include "vector.h";
#include "matrix.h";



namespace engine {

		
		vec3::vec3(float k) {
			x = k;
			y = k;
			z = k;
		} // all values equal
		vec3::vec3(const float x, const float y, const float z) :x(x),y(y),z(z) {}

		vec3::vec3(const float x, const float y, const float z, const float w) 
			: x(x / w), y(y / w), z(z / w) {}

		vec3::vec3(const vec3& v) {
			x = v.x ;
			y = v.y ;
			z = v.z ;
		}

		vec3::vec3(const vec4& v) {
			x = v.x / v.w;
			y = v.y / v.w;
			z = v.z / v.w;
		}

		vec3 operator+(const vec3 & v1, const vec3 & v2){
			return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
		}
		vec3 operator-(const vec3& v1, const vec3& v2) {
			return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
		}
		vec3 operator*(const vec3& v, const float alpha) {
			return vec3(v.x*alpha, v.y*alpha, v.z*alpha);

		}
		vec3 operator*(const float alpha, const vec3& v) {
			return vec3(v.x*alpha, v.y*alpha, v.z*alpha);
		}

		const vec3 vec3::cross(const vec3 & v1, vec3 & v2)
		{	
			mat3 dual = MatrixFactory::createDualMatrice(v1);
			return dual*v2;
		}
		const float vec3::dot(const vec3 & v1, vec3 & v2)
		{
			return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
		}

		std::ostream& operator<<(std::ostream & out, const vec3 & v)
		{
			// TODO: insert return statement here
		}

}