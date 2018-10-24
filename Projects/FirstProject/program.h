#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>
#include <string>

#include "matrix.h"
#include "Shader.h"
#include "camera.h"

#include "GL/glew.h"
#include "GL/freeglut.h"


#define VERTICES 0
#define COLORS 1


class Program {
public:
	GLuint VertexShaderId, FragmentShaderId, ProgramId;
	GLint UniformId, UniformColorId;
	engine::mat4 projMat;
	camera cam;

	Program();
	Program(const char* vertexShader, const char* fragmentShader);
	
	void useProgram();
	void changeCamere();
	void changeProjection(engine::mat4 projectionMatrix);

};


#endif