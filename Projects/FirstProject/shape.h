#pragma once
#ifndef SHAPE_H
#define SHAPE_H
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>
#include <string>

#include "vector.h"
#include "matrix.h"
#include "Shader.h"
#include "program.h"
#include "GL/glew.h"
#include "GL/freeglut.h"

using namespace engine;
typedef struct
{
	GLfloat XYZW[4];

} Vertex;

class Shape {

public:
	GLuint VaoId, VboId[2], UBO_PB;


	Shape() {};
	void createBuffers(const Vertex* v, const GLubyte* i);
	void destroyBuffers();

	void setVertices(const Vertex * v) {
		memcpy(Vertices, v, sizeof(v));
	}

	void setIndices(const GLubyte* i) {
		memcpy(Indices, i, sizeof(i));

	}

	GLuint getVaoId() {
		return VaoId;
	}




	virtual void draw(engine::mat4 transform,const vec4 color, Program prog) = 0;
	
protected:
	Vertex *Vertices;
	GLubyte *Indices;
	GLsizeiptr vSize;
	GLsizeiptr iSize;


};


class Triangle : public Shape {
public:	
	Triangle();
	virtual void draw(engine::mat4 transform, const vec4 color, Program prog);

};

class Square : public Shape {
public:
	Square();
	virtual void draw(engine::mat4 transform, const vec4 color, Program prog);

};

class Parallelogram : public Shape {
public:
	Parallelogram();
	virtual void draw(engine::mat4 transform, const vec4 color, Program prog);

};


#endif
