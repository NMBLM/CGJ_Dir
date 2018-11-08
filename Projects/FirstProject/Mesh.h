#pragma once


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>
#include "vector.h"

#include "GL/glew.h"

namespace engine {

	//typedef struct {
	//	GLfloat x, y, z;
	//} Vertex;

	//typedef struct {
	//	GLfloat u, v;
	//} Texcoord;

	//typedef struct {
	//	GLfloat nx, ny, nz;
	//} Normal;

	class Mesh {
		#define VERTICES 0
		#define TEXCOORDS 1
		#define NORMALS 2

		std::vector <vec3> Vertices, vertexData;
		std::vector <vec2> Texcoords, texcoordData;
		std::vector <vec3> Normals, normalData;
		std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

		void parseVertex(std::stringstream& sin);
		void parseTexcoord(std::stringstream& sin);
		void parseNormal(std::stringstream& sin);
		void parseFace(std::stringstream& sin);
		void parseLine(std::stringstream& sin);
		void loadMeshData(std::string& filename);
		void processMeshData();
		void freeMeshData();

	public:
		bool TexcoordsLoaded, NormalsLoaded;
		GLuint VaoId;
		Mesh();
		Mesh(std::string filename);
		void createMesh(std::string& filename);
		void createBufferObjects();
		void destroyBufferObjects();
		void draw();

	};

}