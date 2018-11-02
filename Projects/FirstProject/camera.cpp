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
	int sideX = (x >= 0) ? 1 : -1;
	int sideY = (y >= 0) ? 1 : -1;
	float mulX = (x > 2 || x < -2) ? 3.0f : 1.5f;
	float mulY = (y > 2 || y < -2) ? 3.0f : 1.5f;
	mulX = (x < 1 && x > -1) ? 0.0f : mulX;
	mulY = (y < 1 && y > -1) ? 0.0f : mulY;
	mat3 rotU = MatrixFactory::createRotationMatrix3(mulX *  sideX *  deltatime * SPEED / 16, u);
	v = rotU * v;
	s = v.cross(u);
	mat3 rotS = MatrixFactory::createRotationMatrix3(mulY * sideY * deltatime * SPEED / 16, s);
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
	std::cout << "Gimbal Lock Mode: "<< ((gLock) ? "True" : "False") << std::endl;
}

mat4 FixedCamera::ViewMatrix()
{
	// recalculate u
	v = vec3(0.0f, 0.0f, 0.0f) - eye;
	v = normalize(v); // normalize
	s = v.cross(u);
	s = normalize(s);  // normalize
	u = s.cross(v);
	u = normalize(u);
	if (gLock) {
		return MatrixFactory::createLookAt(eye, vec3(0.0f, 0.0f, 0.0f), WorldUp);
	}
	else {

		return MatrixFactory::createLookAt(eye, vec3(0.0f, 0.0f, 0.0f), u);
	}
}

void FixedCamera::cameraLookAround(float x, float y, const float deltatime)
{

	int sideX = (x >= 0) ? 1 : -1;
	int sideY = (y >= 0) ? 1 : -1;
	float mulX = (x < 1 && x > -1) ? 0.0f : 1.5f;
	float mulY = (y < 1 && y > -1) ? 0.0f : 1.5f;
	qtrn qX, qY,qZ,q, qV;
	// GIMBAL LOCK ON
	if (gLock) {
		qY = qtrn(sideY * mulY * SPEED  * deltatime, vec4(1.0f, 0.0f, 0.0f, 1.0f));
		WorldUp = qToMatrix(qY) * WorldUp;
		qX = qtrn(sideX * mulX * SPEED  * deltatime, WorldUp);
		WorldSide = qToMatrix(qX) * WorldSide;
	}
	else {
		// GIMBAL LOCK OFF
		qX = qtrn(sideX * mulX * SPEED  * deltatime, u);
		qY = qtrn(sideY * mulY * SPEED  * deltatime, s);
	}
	q = qtrn(0, eye.x, eye.y, eye.z);
	qV = (normalize(qX * qY)) * q * inverse(normalize(qX*qY));
	eye = qToMatrix(qV) * eye;
}

void FixedCamera::cameraMoveRight(const float deltatime)
{
	qtrn qX,q;
	if (gLock) {
		qX = qtrn(2.0f * deltatime * SPEED, WorldUp);
		WorldSide = qToMatrix(qX) * WorldSide;
	}
	else {
		qX = qtrn(2.0f * deltatime * SPEED, u);
	}
	q = qtrn(0, eye.x, eye.y, eye.z);
	eye = qToMatrix(qX * q * inverse(qX)) * eye;
}

void FixedCamera::cameraMoveLeft(const float deltatime)
{
	qtrn qX, q;
	if (gLock) {
		qX = qtrn(-2.0f * deltatime * SPEED, WorldUp);
		WorldSide = qToMatrix(qX) * WorldSide;
	}
	else {
		qX = qtrn(-2.0f * deltatime * SPEED, u);
	}
	q = qtrn(0, eye.x, eye.y, eye.z);
	eye = qToMatrix(qX * q * inverse(qX)) * eye;
}

void FixedCamera::cameraMoveForward(const float deltatime)
{
	qtrn qY,q;
	if (gLock) {
		qY = qtrn(-2.0f * deltatime * SPEED, vec4(1.0f, 0.0f, 0.0f, 1.0f));
		WorldUp = qToMatrix(qY) * WorldUp;
	}
	else {
		qY = qtrn(-2.0f * deltatime * SPEED, s);
	}
	q = qtrn(0, eye.x, eye.y, eye.z);
	eye = qToMatrix(qY * q * inverse(qY)) * eye;
}

void FixedCamera::cameraMoveBack(const float deltatime)
{
	qtrn qY, q;
	if (gLock) {
		qY = qtrn(2.0f * deltatime * SPEED, vec4(1.0f, 0.0f, 0.0f, 1.0f));
		WorldUp = qToMatrix(qY) * WorldUp;
	}
	else {
		qY = qtrn(2.0f * deltatime * SPEED, s);
	}
	q = qtrn(0, eye.x, eye.y, eye.z);
	eye = qToMatrix(qY * q * inverse(qY)) * eye;
}

void FixedCamera::cameraRollRight(const float deltatime)
{
	qtrn qZ;
	if (gLock) {
		qZ = qtrn(2.0f * deltatime * SPEED, WorldSide.cross(WorldUp));
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
		qZ = qtrn(-2.0f * deltatime * SPEED, WorldSide.cross(WorldUp));
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