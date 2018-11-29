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

#include "matrix.h"

#include "GL/glew.h"
#include "GL/freeglut.h"
using namespace engine;
class ShaderProgram {
public:
	GLuint id;
	std::map<std::string, GLint> shaderid = std::map< std::string, GLint>();
	std::map<std::string, mat4> unimat4 = std::map< std::string, mat4>();
	std::map<std::string, vec4> univec4 = std::map< std::string, vec4>();
	std::map<std::string, vec3> univec3 = std::map< std::string, vec3>();
	std::map<std::string, vec2> univec2 = std::map< std::string, vec2>();
	std::map<std::string, float> unifloat = std::map< std::string, float>();
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
	void addUniformVec(const char* name, vec4 v);
	void addUniformVec(const char* name, vec3 v);
	void addUniformVec(const char* name, vec2 v);
	void addUniformFloat(const char* name, float v);
	void addUniformMat(const char* name, mat4 v);
	void use();
	void stop();


};

#endif