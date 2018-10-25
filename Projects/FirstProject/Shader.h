#pragma once
#ifndef SHADER_H
#define SHADER_H

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




class Shader {
	public:
		GLuint id;

		void deleteShader();

};

class VertexShader: public Shader {
public:


	VertexShader();
	VertexShader(const char* shaderFilename);


};

class FragmentShader : public Shader {
public:


	FragmentShader();
	FragmentShader(const char* shaderFilename);


};


#endif