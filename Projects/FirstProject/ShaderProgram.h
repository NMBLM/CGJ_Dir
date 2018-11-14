#pragma once
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>
#include <string>

#include <map>
#include <iterator>

#include "matrix.h"
#include "shader.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

class ShaderProgram {
public:
	GLuint id;
	std::map<std::string, int> shaderid = std::map< std::string, int>();
	ShaderProgram();
	ShaderProgram(GLuint i);

	void attachShader(GLuint type,const char* name, const char* filename);
	void detachShader(const char* name);
	void link();
	void bindAttribLocation(const int atnum, const char* atname);
	void uniformBlockBinding(GLint bid, GLuint bpo);
	GLint UniformId(const char* uniformName);
	GLint UniformLocation(const char* uniformName);
	GLint uniformBlockIndex(const char* uniformName);


};

#endif