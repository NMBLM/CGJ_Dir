#include "camera.h"
#include "vector.h"
#include "matrix.h"

using namespace engine;


Camera::Camera()
{
}

Camera::Camera(const vec3 eye, const vec3 center, const vec3 up)
{
	this->eye = eye;
	v = center - eye;
	v = v * (1 / v.length()); // normalize
	s = v.cross(up);
	s = s * (1 / s.length());  // normalize
	u = s.cross(v); //is a unit vector only because s and v are perpendicular to each other and are unit vectors |s x v| = |s||v|*sin(angle)
}

mat4 Camera::ViewMatrix()
{

	//return MatrixFactory::createLookAt(eye, v, u, s);
	return MatrixFactory::createLookAt(eye, eye + v, u);

}

void Camera::cameraLookAround(float x, float y, const float deltatime)
{
	int sideX = (x >=0 )? 1 : -1 ;
	int sideY = (y >= 0) ? 1 : -1;
	float mulX = (x > 2 || x < -2) ? 3.0f : 1.5f;
	float mulY = (y > 2 || y < -2) ? 3.0f : 1.5f;
	mulX = (x < 1 && x > -1) ? 0.0f : mulX;
	mulY = (y < 1 && y > -1) ? 0.0f : mulY;
	mat3 rotU = MatrixFactory::createRotationMatrix3( mulX *  sideX *  deltatime * SPEED / 16, u);
	v = rotU * v;
	s = v.cross(u);
	mat3 rotS = MatrixFactory::createRotationMatrix3( mulY * sideY * deltatime * SPEED / 16, s);
	v = rotS * v;
	u = s.cross(v);
	v = normalize(v);
	s = normalize(s);
	u = normalize(u);
}

void Camera::cameraMoveRight(const float deltatime)
{
	eye = eye + s * SPEED * deltatime;

}

void Camera::cameraMoveLeft(const float deltatime)
{
	eye = eye - s * SPEED * deltatime;

}

void Camera::cameraMoveForward(const float deltatime)
{
	eye = eye + v * SPEED * deltatime;

}

void Camera::cameraMoveBack(const float deltatime)
{
	eye = eye - v * SPEED * deltatime;

}



/*float xoffset = xpos - lastX;
float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
lastX = xpos;
lastY = ypos;

float sensitivity = 0.05f;
xoffset *= sensitivity;
yoffset *= sensitivity;*/