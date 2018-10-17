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
		GLuint VertexShaderId, FragmentShaderId, ProgramId;
		GLint UniformId, UniformColorId;

		Shader();
		Shader(const char* vertexShader, const char* fragmentShader);

		void useProgram();



};








#endif