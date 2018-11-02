#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

using namespace engine;


class Camera {

	protected:
		vec3 v;
		vec3 s;
		vec3 u;
		vec3 eye;
		const float SPEED = 10.0f;
		const float vSPEED = 0.5f;
		float lastFrame = 0.0f;

	public:
		Camera();
		Camera(const vec3 eye,const vec3 center,const vec3 up);

		mat4 ViewMatrix();
		void cameraLookAround(float x, float y, const float deltatime);
		void cameraMoveRight(const float deltatime);
		void cameraMoveLeft(const float deltatime);
		void cameraMoveForward(const float deltatime);
		void cameraMoveBack(const float deltatime);

};

class FixedCamera : public Camera {
	public:
		bool gLock = true;
		vec3 WorldUp = vec3(0.0f, 1.0f, 0.0f);
		vec3 WorldSide = vec3(1.0f, 0.0f, 0.0f);
		vec3 WorldZ = vec3(0.0f, 0.0f, 1.0f);


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