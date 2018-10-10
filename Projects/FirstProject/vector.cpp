#include "vector.h"
#include "matrix.h"


const bool fcmp(float a, float b, float epsilon = 0.00005f) {
	return fabs(a - b) < epsilon;
}

namespace engine {

		//BEGIN vec3 constructor
		vec3::vec3(float k) 
		{ // all values equal
			x = k; y = k; z = k;
		}

		vec3::vec3(const float x, const float y, const float z) :x(x),y(y),z(z) {}

		vec3::vec3(const float x, const float y, const float z, const float w) 
			: x(x / w), y(y / w), z(z / w) {}

		vec3::vec3(const vec3& v) 
		{
			x = v.x ; y = v.y ; z = v.z ; 
		}

		vec3::vec3(const vec4& v)
		{
			x = v.x / v.w;
			y = v.y / v.w;
			z = v.z / v.w;
		}
		//END vec3 constructor
			
		
		vec3 vec3::operator+=(const vec3 v)
		{
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
			return *this;

		}

		vec3 vec3::operator-=(const vec3 v)
		{
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
			return *this;
		}

		vec3 vec3::operator*=(const float alpha)
		{
			this->x *= alpha;
			this->y *= alpha;
			this->z *= alpha;
			return *this;

		}

		const bool operator==(const vec3 & v1, const vec3 & v2)
		{
			return fcmp(v1.x, v2.x) && fcmp(v1.y, v2.y) && fcmp(v1.z, v2.z);
		}

		const bool operator!=(const vec3 & v1, const vec3 & v2)
		{
			return !( fcmp(v1.x, v2.x) && fcmp(v1.y, v2.y) && fcmp(v1.z, v2.z) );
		}

		vec3 operator+(const vec3 & v1, const vec3 & v2)
		{
			return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
		}
		vec3 operator-(const vec3& v1, const vec3& v2)
		{
			return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
		}
		vec3 operator*(const vec3& v, const float alpha) 
		{
			return vec3(v.x*alpha, v.y*alpha, v.z*alpha);

		}
		vec3 operator*(const float alpha, const vec3& v) {
			return vec3(v.x*alpha, v.y*alpha, v.z*alpha);
		}

		const vec3 vec3::cross(const vec3 & v2)
		{	
			mat3 dual = MatrixFactory::createDualMatrix(*this);
			return dual*v2;
		}
		const float vec3::dot(const vec3 & v2)
		{
			return x*v2.x + y*v2.y + z*v2.z;
		}

		std::ostream& operator<<(std::ostream & out, const vec3 & v)
		{
			out << "( " << v.x << " , " << v.y << " , " << v.z << " )";
			return out;
		}

		std::istream & operator>>(std::istream & in, vec3 & v)
		{
			in >> v.x >> v.y >> v.z;
			return in;
		}

}
