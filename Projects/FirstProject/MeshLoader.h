#pragma once
#ifndef MESHLOADER_H
#define MESHLOADER_H

#include<iostream>
#include "GL/glew.h"
#include <vector>
#include "Mesh.h"
#include "Vector.h"
#include <fstream>
#include <sstream>

namespace engine {

	class MeshLoader {
	public:

		std::vector <vec3> vertexData;
		std::vector <vec2> texcoordData;
		std::vector <vec3> normalData;

		std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

		bool TexcoordsLoaded, NormalsLoaded;

		MeshLoader();
		~MeshLoader();

		Mesh createMesh(const std::string& filename);

	private:
		void parseVertex(std::stringstream& sin);

		void parseTexcoord(std::stringstream& sin);

		void parseNormal(std::stringstream& sin);

		void parseFace(std::stringstream& sin);

		void parseLine(std::stringstream& sin);

		void loadMeshData(const std::string& filename);

		Mesh processMeshData();

		void freeMeshData();

	};

}

#endif // !MESHLOADER_H


