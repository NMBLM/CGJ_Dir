#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"


#define VERTICES 0
#define COLORS 1


class Program {
public:
	GLuint VertexShaderId, FragmentShaderId, ProgramId;
	GLint UniformId, UniformColorId;

	Program();
	Program(const char* vertexShader, const char* fragmentShader);
	
	void useProgram();

};


#endif