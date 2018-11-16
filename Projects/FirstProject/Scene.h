#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "vector.h"
#include "Mesh.h"
#include "camera.h"
#include "matrix.h"
#include "Anime.h"
#include "ShaderProgram.h"

#include "GL/glew.h"


namespace engine {



	class SceneNode {
		Mesh* mesh = nullptr;
		ShaderProgram* shaderProgram = nullptr;
		qtrn qDir = qtrn();
		mat4 model = MatrixFactory::createIdentityMatrix4();
		std::vector <SceneNode*> nodes;
		vec4 color = vec4(-1, 0, 0, 1);
		Animator* anime = nullptr;

	public:
		SceneNode();
		SceneNode(Mesh* m = nullptr, ShaderProgram* shaders= nullptr, mat4 model = MatrixFactory::createIdentityMatrix4());
		void draw(float delta, mat4 fm);
		void updateModel(mat4 trs);
		bool hasShaderProgram();
		ShaderProgram * getShaderProgram();
		void addNode(SceneNode* node);
		void setColor(vec4 c);

		void addAnimator(Animator* a);
		Animator* getAnimator();
		void resetAnimator();
	};


	class Scene {
		SceneNode* root = nullptr;
		ShaderProgram* dfault;
		// nothing for now
		Camera* camera;
		GLuint VboId[1];

	public:
		Scene(ShaderProgram* shaders,Camera* cam);
		void draw(float delta);
		void addNode(SceneNode* node);
		void updateModel(mat4 trs);
		void resetAnimator();

	};

}

#endif
