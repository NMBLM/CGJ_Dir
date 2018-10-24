#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "matrix.h"

using namespace engine;


class Camera {

	engine::vec3 v;
	engine::vec3 s;
	engine::vec3 u;
	engine::vec3 eye;

	const float SPEED = 2;
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



#endif