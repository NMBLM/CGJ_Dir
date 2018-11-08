#include "vector.h"
#include "matrix.h"
#include "quaternion.h"




namespace engine {

	// VECTOR 2 ////////////////////////////////////////////////////
	const bool fcmp(float a, float b, float epsilon)
	{
		return fabs(a - b) < epsilon;
	}

	vec2::vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	vec2::vec2(const float k): x(k), y(k)
	{
	}

	vec2::vec2(const float x, const float y) : x(x), y(y)
	{
	}

	vec2::vec2(const vec2 & v)
	{
		x = v.x;
		y = v.y;
	}

	float * vec2::data()
	{
		float *data = new float[2]();
		data[0] = x;
		data[1] = y;
		return data;
	}

	vec2 vec2::operator=(const vec2 & v)
	{
		x = v.x; y = v.y;
		return *this;
	}
	vec2 vec2::operator+=(const vec2 v)
	{
		this->x += v.x;
		this->y += v.y;
		return *this;

	}

	vec2 vec2::operator-=(const vec2 v)
	{
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	}

	vec2 vec2::operator*=(const float alpha)
	{
		this->x *= alpha;
		this->y *= alpha;
		return *this;
	}

	const float vec2::length() const
	{
		return sqrt(x * x + y * y);
	}

	const float vec2::quadrance() const
	{
		return x * x + y * y;
	}

	
	const bool operator==(const vec2 & v1, const vec2 & v2)
	{
		return fcmp(v1.x, v2.x) && fcmp(v1.y, v2.y);
	}

	const bool operator!=(const vec2 & v1, const vec2 & v2)
	{
		return !(fcmp(v1.x, v2.x) && fcmp(v1.y, v2.y));
	}

	vec2 operator+(const vec2 & v1, const vec2 & v2)
	{
		return vec2(v1.x + v2.x, v1.y + v2.y);
	}
	vec2 operator-(const vec2& v1, const vec2& v2)
	{
		return vec2(v1.x - v2.x, v1.y - v2.y);
	}
	vec2 operator*(const vec2& v, const float alpha)
	{
		return vec2(v.x*alpha, v.y*alpha);

	}
	vec2 operator*(const float alpha, const vec2& v) {
		return vec2(v.x*alpha, v.y*alpha);
	}

	std::ostream& operator<<(std::ostream& out, const vec2 & v)
	{
		out << "( " << v.x << " , " << v.y <<  " )";
		return out;
	}

	std::istream & operator>>(std::istream& in, vec2 & v)
	{
		in >> v.x >> v.y;
		return in;
	}

	// VECTOR 3 ////////////////////////////////////////////////////

	vec3::vec3() :x(0), y(0), z(0)
	{
	}

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
	const float * vec3::data(vec3 & v)
	{
		float *data = new float[3]();
		data[0] = x;
		data[1] = y;
		data[2] = z;
		return data;
	}

	void vec3::clean()
	{
		x = fcmp(x, 0.0f) ? 0.0f : x;
		y = fcmp(y, 0.0f) ? 0.0f : y;
		z = fcmp(z, 0.0f) ? 0.0f : z;

	}
		
	vec3 vec3::operator=(const vec3 & v)
	{
		x = v.x; y = v.y; z = v.z;
		return *this;
	}

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

	const vec3 normalize(const vec3 & v)
	{
		vec3 res = v * (1 / v.length());
		res.clean();
		return res;
	}

	const vec3 vec3::cross(const vec3& v2) const
	{	
		mat3 dual = MatrixFactory::createDualMatrix(*this);
		return dual*v2;
	}
	const float vec3::dot(const vec3& v2) const
	{
		return x*v2.x + y*v2.y + z*v2.z;
	}

	const float vec3::length() const 
	{
		return sqrt(x * x + y * y + z * z);
	}

	const float vec3::quadrance() const
	{
		return x * x + y * y + z * z;
	}

	std::ostream& operator<<(std::ostream& out, const vec3 & v)
	{
		out << "( " << v.x << " , " << v.y << " , " << v.z << " )";
		return out;
	}

	std::istream & operator>>(std::istream& in, vec3 & v)
	{
		in >> v.x >> v.y >> v.z;
		return in;
	}

	// VECTOR 4 ////////////////////////////////////////////////////

	vec4::vec4()
	{
		float k = 1.0f;
		x = k; y = k; z = k; w = 1;
	}

	vec4::vec4(float k)
	{ // all values equal
		x = k; y = k; z = k; w=1;
	}

	vec4::vec4(const float x, const float y, const float z) :x(x), y(y), z(z), w(1) {}

	vec4::vec4(const float x, const float y, const float z, const float w)
		: x(x / w), y(y / w), z(z / w) , w(1){}


	vec4::vec4(const vec3& v, const float _w)
	{
		x = v.x; y = v.y; z = v.z; w = _w;
	}

	vec4::vec4(const vec4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	const float * vec4::data() const 
	{
		float *data = new float[4]();
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;
		return data;
	}

	vec4 vec4::operator=(const vec4 v)
	{
		return vec4(v.x,v.y,v.z,v.w);
	}


	const bool operator==(const vec4 & v1, const vec4 & v2)
	{
		return fcmp(v1.x, v2.x) && fcmp(v1.y, v2.y) && fcmp(v1.z, v2.z) && fcmp(v1.w, v2.w);
	}

	const bool operator!=(const vec4 & v1, const vec4 & v2)
	{
		return !(fcmp(v1.x, v2.x) && fcmp(v1.y, v2.y) && fcmp(v1.z, v2.z) && fcmp(v1.w, v2.w));
	}

	vec4 operator*(const vec4 & v, const float alpha)
	{
		return vec4(v.x*alpha, v.y*alpha, v.z*alpha,v.w);
	}

	vec4 operator*(const float alpha, const vec4 & v)
	{
		return vec4(v.x*alpha, v.y*alpha, v.z*alpha, v.w);
	}


	std::ostream& operator<<(std::ostream& out, const vec4 & v)
	{
		out << "( " << v.x << " , " << v.y << " , " << v.z << " , " << v.w << " )";
		return out;
	}

	std::istream & operator>>(std::istream& in, vec4 & v)
	{
		in >> v.x >> v.y >> v.z >> v.w;
		return in;
	}
}
