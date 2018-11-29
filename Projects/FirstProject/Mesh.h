//#pragma once
//#ifndef MESH_H
//#define MESH_H
//
//#include <vector>
//#include "GL/glew.h"
//#include "Matrix.h"
//
//#define VERTICES 0
//#define TEXCOORDS 1
//#define NORMALS 2
//
//namespace engine {
//
//	class Mesh {
//		GLuint VaoId;
//	public:
//
//		static constexpr const char* const TABLE = "TABLE";
//		static constexpr const char* const GROUND = "GROUND";
//		static constexpr const char* const TRIANGLE = "TRIANGLE";
//		static constexpr const char* const SQUARE = "SQUARE";
//		static constexpr const char* const PARALLELOGRAM = "PARALLELOGRAM";
//		static constexpr const char* const DUCK = "DUCK";
//
//		bool TexcoordsLoaded, NormalsLoaded;
//		std::vector <vec3>* vertices;
//		std::vector <vec2>* texcoords;
//		std::vector <vec3>* normals;
//
//		Mesh() = default;
//		Mesh(std::vector<vec3> vertices,
//			std::vector <vec2> texcoords,
//			std::vector <vec3> normals,
//			bool tcLoaded, bool nLoaded);
//		~Mesh() = default;
//
//		void destroyBufferObjects();
//		void draw();
//		void createBufferObjects();
//	};
//}
//#endif // !MESH_H

#pragma once


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "vector.h"
#include "matrix.h"
#include "ShaderProgram.h"

#include "GL/glew.h"

namespace engine {

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
		Mesh(std::vector<vec3>vertices,
			std::vector <vec2> texcoords,
			std::vector <vec3> normals,
			bool tcLoaded, bool nLoaded);
		Mesh(std::string filename);
		void createMesh(std::string& filename);
		void createBufferObjects();
		void destroyBufferObjects();
		void draw();
		void draw(mat4 transform, ShaderProgram* prog);
		void draw(mat4 transform, ShaderProgram* prog, vec4 color);

	};

}