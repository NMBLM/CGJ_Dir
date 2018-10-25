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
#include "shader.h"
#include "camera.h"

#include "GL/glew.h"
#include "GL/freeglut.h"


class Program {
public:
	GLuint id;

	Program();
	Program(GLuint i);
	
	void attachShader(Shader shader);
	void detachShader(Shader shader);
	void link();
	void bindAttribLocation(const int atnum, const char* atname);
	GLint UniformId(const char* uniformName);
	GLint UniformLocation(const char* uniformName);
	GLint uniformBlockIndex(const char* uniformName);


};


#endif