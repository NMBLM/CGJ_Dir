//#include "MeshLoader.h"
//
////TODO singleton
//namespace engine {
//
//	MeshLoader::MeshLoader() {
//	}
//
//	MeshLoader::~MeshLoader() {
//	}
//
//	Mesh MeshLoader::createMesh(const std::string& filename) {
//		loadMeshData(filename);
//		Mesh m = processMeshData();
//		freeMeshData();
//
//		return m;
//	}
//
//	void MeshLoader::loadMeshData(const std::string& filename) {
//		std::ifstream ifile(filename);
//		std::string line;
//		while (std::getline(ifile, line)) {
//			std::stringstream s = std::stringstream(line);
//			parseLine(s);
//		}
//		TexcoordsLoaded = (texcoordIdx.size() > 0);
//		NormalsLoaded = (normalIdx.size() > 0);
//	}
//
//	void MeshLoader::parseLine(std::stringstream & sin) {
//		std::string s;
//		sin >> s;
//		if (s.compare("v") == 0) parseVertex(sin);
//		else if (s.compare("vt") == 0) parseTexcoord(sin);
//		else if (s.compare("vn") == 0) parseNormal(sin);
//		else if (s.compare("f") == 0) parseFace(sin);
//	}
//
//	void MeshLoader::parseVertex(std::stringstream & sin) {
//		vec3 v;
//		sin >> v;
//		vertexData.push_back(v);
//	}
//
//	void MeshLoader::parseTexcoord(std::stringstream & sin) {
//		vec2 t;
//		sin >> t;
//		texcoordData.push_back(t);
//	}
//
//	void MeshLoader::parseNormal(std::stringstream & sin) {
//		vec3 n;
//		sin >> n;
//		normalData.push_back(n);
//	}
//
//	void MeshLoader::parseFace(std::stringstream & sin) {
//		std::string token;
//		for (int i = 0; i < 3; i++) {
//			std::getline(sin, token, '/');
//			if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
//			std::getline(sin, token, '/');
//			if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
//			std::getline(sin, token, ' ');
//			if (token.size() > 0) normalIdx.push_back(std::stoi(token));
//		}
//	}
//
//	Mesh MeshLoader::processMeshData() {
//		//TODO fazer de forma mais eficiente
//		std::vector <vec3> Vertices;
//		std::vector <vec2> Texcoords;
//		std::vector <vec3> Normals;
//
//		for (unsigned int i = 0; i < vertexIdx.size(); i++) {
//			unsigned int vi = vertexIdx[i];
//			vec3 v = vertexData[vi - 1];
//			Vertices.push_back(v);
//			if (TexcoordsLoaded) {
//				unsigned int ti = texcoordIdx[i];
//				vec2 t = texcoordData[ti - 1];
//				Texcoords.push_back(t);
//			}
//			if (NormalsLoaded) {
//				unsigned int ni = normalIdx[i];
//				vec3 n = normalData[ni - 1];
//				Normals.push_back(n);
//			}
//		}
//		return Mesh(Vertices, Texcoords, Normals, TexcoordsLoaded, NormalsLoaded);
//	}
//
//	void MeshLoader::freeMeshData() {
//		vertexData.clear();
//		texcoordData.clear();
//		normalData.clear();
//		vertexIdx.clear();
//		texcoordIdx.clear();
//		normalIdx.clear();
//	}
//}
#include "MeshLoader.h"

//TODO singleton
namespace engine {

	MeshLoader::MeshLoader() {
	}

	MeshLoader::~MeshLoader() {
	}

	Mesh* MeshLoader::createMesh(const std::string& filename) {
		loadMeshData(filename);
		Mesh* m = processMeshData();
		freeMeshData();

		return m;
	}

	void MeshLoader::loadMeshData(const std::string& filename) {
		std::ifstream ifile(filename);
		std::string line;
		while (std::getline(ifile, line)) {
			std::stringstream s = std::stringstream(line);
			parseLine(s);
		}
		TexcoordsLoaded = (texcoordIdx.size() > 0);
		NormalsLoaded = (normalIdx.size() > 0);
	}

	void MeshLoader::parseLine(std::stringstream & sin) {
		std::string s;
		sin >> s;
		if (s.compare("v") == 0) parseVertex(sin);
		else if (s.compare("vt") == 0) parseTexcoord(sin);
		else if (s.compare("vn") == 0) parseNormal(sin);
		else if (s.compare("f") == 0) parseFace(sin);
	}

	void MeshLoader::parseVertex(std::stringstream & sin) {
		vec3 v;
		sin >> v;
		vertexData.push_back(v);
	}

	void MeshLoader::parseTexcoord(std::stringstream & sin) {
		vec2 t;
		sin >> t;
		texcoordData.push_back(t);
	}

	void MeshLoader::parseNormal(std::stringstream & sin) {
		vec3 n;
		sin >> n;
		normalData.push_back(n);
	}

	void MeshLoader::parseFace(std::stringstream & sin) {
		std::string token;
		for (int i = 0; i < 3; i++) {
			std::getline(sin, token, '/');
			if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
			std::getline(sin, token, '/');
			if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
			std::getline(sin, token, ' ');
			if (token.size() > 0) normalIdx.push_back(std::stoi(token));
		}
	}

	Mesh* MeshLoader::processMeshData() {
		//TODO fazer de forma mais eficiente
		std::vector <vec3> Vertices;
		std::vector <vec2> Texcoords;
		std::vector <vec3> Normals;

		for (unsigned int i = 0; i < vertexIdx.size(); i++) {
			unsigned int vi = vertexIdx[i];
			vec3 v = vertexData[vi - 1];
			Vertices.push_back(v);
			if (TexcoordsLoaded) {
				unsigned int ti = texcoordIdx[i];
				vec2 t = texcoordData[ti - 1];
				Texcoords.push_back(t);
			}
			if (NormalsLoaded) {
				unsigned int ni = normalIdx[i];
				vec3 n = normalData[ni - 1];
				Normals.push_back(n);
			}
		}
		return new Mesh(Vertices, Texcoords, Normals, TexcoordsLoaded, NormalsLoaded);
	}

	void MeshLoader::freeMeshData() {
		vertexData.clear();
		texcoordData.clear();
		normalData.clear();
		vertexIdx.clear();
		texcoordIdx.clear();
		normalIdx.clear();
	}
}
