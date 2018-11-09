#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "vector.h"
#include "Mesh.h"
#include "matrix.h"
#include "program.h"

#include "GL/glew.h"


namespace engine {



	class SceneNode {
		Mesh* mesh = nullptr;
		Program* shaderProgram = nullptr;
		mat4 model = MatrixFactory::createIdentityMatrix4();
		std::vector <SceneNode*> nodes;

		Program * getShaderProgram();

	public:
		SceneNode();
		void draw(float delta);
		void updateModel(mat4 trs);
		bool hasShaderProgram();
		void addNode(SceneNode* node);
	};


	class Scene {
		SceneNode* root = nullptr;
	public:
		Scene();
		void draw(float delta);
		void addNode(SceneNode* node);
	};

}

#endif // SCENE_H
