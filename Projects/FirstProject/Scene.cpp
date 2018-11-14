#include "Scene.h"

using namespace engine;


engine::Scene::Scene(ShaderProgram* shaders)
{
	root = new SceneNode(nullptr, shaders);
}

void engine::Scene::draw(float delta)
{
	root->draw(delta);
}

void engine::Scene::addNode(SceneNode * node)
{
	root->addNode(node);
}



engine::SceneNode::SceneNode()
{
	nodes = std::vector <SceneNode*>();
}

engine::SceneNode::SceneNode(Mesh * m, ShaderProgram * shaders, mat4 mat)
{
	mesh = m;
	shaderProgram = shaders;
	model = mat;
	nodes = std::vector <SceneNode*>();
}

void engine::SceneNode::draw(float delta)
{
	if (mesh != nullptr) {
		mesh->draw(model,this->getShaderProgram());
	}
	for (auto& s : nodes) {
		s->draw(delta);
	}
}

void engine::SceneNode::updateModel(mat4 trs)
{
	model = trs * model;
	for (auto& s : nodes) {
		s->updateModel(trs);
	}
}

bool engine::SceneNode::hasShaderProgram()
{
	return shaderProgram != nullptr;
}

void engine::SceneNode::addNode(SceneNode * node)
{
	if (!node->hasShaderProgram()) {
		node->shaderProgram = this->shaderProgram;
	}
	nodes.push_back(node);
}

ShaderProgram * engine::SceneNode::getShaderProgram() {
	if (shaderProgram != nullptr) {
		return shaderProgram;
	}
	std::cout << "CRASH AND BURN on getShaderProgram" << std::endl;
	exit(1203124);
}


