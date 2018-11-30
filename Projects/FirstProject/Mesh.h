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

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

namespace engine{

	class Mesh{


		std::vector <vec3> Vertices, vertexData;
		std::vector <vec2> Texcoords, texcoordData;
		std::vector <vec3> Normals, normalData;
		std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

		void parseVertex( std::stringstream& sin );
		void parseTexcoord( std::stringstream& sin );
		void parseNormal( std::stringstream& sin );
		void parseFace( std::stringstream& sin );
		void parseLine( std::stringstream& sin );
		void loadMeshData( std::string& filename );
		void processMeshData();
		void freeMeshData();

		public:
		bool TexcoordsLoaded, NormalsLoaded;
		GLuint VaoId;
		Mesh();
		Mesh( std::vector<vec3>vertices,
			std::vector <vec2> texcoords,
			std::vector <vec3> normals,
			bool tcLoaded, bool nLoaded );
		Mesh( std::string filename );
		void createMesh( std::string& filename );
		void createBufferObjects();
		void destroyBufferObjects();
		void draw();
		void draw( mat4 transform, ShaderProgram* prog );
		void draw( mat4 transform, ShaderProgram* prog, vec4 color );

	};

}