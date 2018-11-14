#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

using namespace engine;


class Camera {
private:
	vec3 v;
	vec3 s;
	vec3 u;
	protected:
		mat4 projection;
		vec3 eye;
		const float SPEED = 2.0f;
		const float vSPEED = 1.0f;
		float lastFrame = 0.0f;

	public:
		Camera();
		Camera(const vec3 eye,const vec3 center,const vec3 up);

		mat4 ViewMatrix();
		mat4 ProjectionMatrix();

		void ProjectionMatrix(mat4 proj);
		void cameraLookAround(float x, float y, const float deltatime);
		void cameraMoveRight(const float deltatime);
		void cameraMoveLeft(const float deltatime);
		void cameraMoveForward(const float deltatime);
		void cameraMoveBack(const float deltatime);

};

class FixedCamera : public Camera {
	public:
		bool gLock = false;
		int print = 0;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float roll = 0.0f;
		qtrn qPos;

		FixedCamera();
		FixedCamera(const vec3 eye, const vec3 center, const vec3 up);

		void gimbalLockSwitch();
		mat4 ViewMatrix();
		void cameraLookAround(float x, float y, const float deltatime);
		void cameraMoveRight(const float deltatime);
		void cameraMoveLeft(const float deltatime);
		void cameraMoveForward(const float deltatime);
		void cameraMoveBack(const float deltatime);
		void cameraRollRight(const float deltatime);
		void cameraRollLeft(const float deltatime);
		void zoom(const int dir, const float deltatime);

};

#endif