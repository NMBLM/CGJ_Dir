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
	eye = eye + s * vSPEED * deltatime;

}

void Camera::cameraMoveLeft(const float deltatime)
{
	eye = eye - s * vSPEED * deltatime;

}

void Camera::cameraMoveForward(const float deltatime)
{
	eye = eye + v * vSPEED * deltatime;

}

void Camera::cameraMoveBack(const float deltatime)
{
	eye = eye - v * vSPEED * deltatime;
}



// FIXED CAMERA


FixedCamera::FixedCamera()
{
}

FixedCamera::FixedCamera(const vec3 eye, const vec3 center, const vec3 up)
{
	this->eye = eye;
	v = center - eye;
	v = v * (1 / v.length()); // normalize
	s = v.cross(up);
	s = s * (1 / s.length());  // normalize
	u = s.cross(v); //is a unit vector only because s and v are perpendicular to each other and are unit vectors |s x v| = |s||v|*sin(angle)
}


void FixedCamera::gimbalLockSwitch()
{
	gLock = !gLock;
}

mat4 FixedCamera::ViewMatrix()
{
	// recalculate u
	v = vec3(0.0f, 0.0f, 0.0f) - eye;
	v = v * (1 / v.length()); // normalize
	s = v.cross(u);
	s = s * (1 / s.length());  // normalize
	u = s.cross(v);
	return MatrixFactory::createLookAt(eye,vec3(0.0f,0.0f,0.0f), u);
	//return MatrixFactory::createLookAt(eye, eye + v, u);


}

void FixedCamera::cameraLookAround(float x, float y, const float deltatime)
{

	int sideX = (x >= 0) ? 1 : -1;
	int sideY = (y >= 0) ? 1 : -1;
	float mulX = (x > 2 || x < -2) ? 3.0f : 1.5f;
	float mulY = (y > 2 || y < -2) ? 3.0f : 1.5f;
	mulX = (x < 1 && x > -1) ? 0.0f : mulX;
	mulY = (y < 1 && y > -1) ? 0.0f : mulY;
	qtrn qX, qY;
	// GIMBAL LOCK ON
	if (gLock) {
		 qX = qtrn(sideX * mulX * SPEED * deltatime , vec4(0.0f, 1.0f, 0.0f, 1.0f));
		 qY = qtrn(sideY * mulY * SPEED * deltatime , vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else {
		// GIMBAL LOCK OFF
		qX = qtrn(sideX * mulX * SPEED * deltatime , u);
		qY = qtrn(sideY * mulY * SPEED * deltatime , s);
	}
	eye = qToMatrix(qX + qY) *  eye;

}

void FixedCamera::cameraMoveRight(const float deltatime)
{
	qtrn qX;
	if (gLock) {
		qX = qtrn(2.0f * deltatime * SPEED, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else {
		qX = qtrn(2.0f * deltatime * SPEED, u);
	}
	eye = qToMatrix(qX) * eye;
}

void FixedCamera::cameraMoveLeft(const float deltatime)
{
	qtrn qX;
	if (gLock) {
		qX = qtrn(-2.0f * deltatime * SPEED, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else {
		qX = qtrn(-2.0f * deltatime * SPEED, u);
	}
	eye = qToMatrix(qX) * eye;
}

void FixedCamera::cameraMoveForward(const float deltatime)
{
	qtrn qY;
	if (gLock) {
		qY = qtrn(-2.0f * deltatime * SPEED, vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else {
		qY = qtrn(-2.0f * deltatime * SPEED, s);
	}
	eye = qToMatrix(qY) * eye;
}

void FixedCamera::cameraMoveBack(const float deltatime)
{
	qtrn qY;
	if (gLock) {
		qY = qtrn(2.0f * deltatime * SPEED, vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else {
		qY = qtrn(2.0f * deltatime * SPEED, s);
	}
	eye = qToMatrix(qY) * eye;
}

void FixedCamera::cameraRollRight(const float deltatime)
{
	qtrn qZ;
	if (gLock) {
		qZ = qtrn(2.0f * deltatime * SPEED, vec4(0.0f, 0.0f, -1.0f, 1.0f));
	}
	else {
		qZ = qtrn(2.0f * deltatime * SPEED, v);
	}
	u = qToMatrix(qZ) * u;
	u = normalize(u);

}

void FixedCamera::cameraRollLeft(const float deltatime)
{
	qtrn qZ;
	if (gLock) {
		qZ = qtrn(-2.0f * deltatime * SPEED, vec4(0.0f, 0.0f, -1.0f, 1.0f));
	}
	else {
		qZ = qtrn(-2.0f * deltatime * SPEED, v);
	}
	u = qToMatrix(qZ) * u;
	u = normalize(u);
}


void FixedCamera::zoom(const int dir, const float deltatime)
{
	if (dir > 0) {
		eye = eye + v * vSPEED * deltatime;
	}
	else {
		eye = eye - v * vSPEED * deltatime;
	}
}
