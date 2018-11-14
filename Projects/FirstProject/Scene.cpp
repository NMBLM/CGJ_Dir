#include "Scene.h"

using namespace engine;


engine::Scene::Scene(ShaderProgram* shaders,Camera* cam)
{
	root = new SceneNode(nullptr, shaders);
	camera = cam;

	//glGenBuffers(1, VboId);

	//glBindBuffer(GL_UNIFORM_BUFFER, VboId[0]);
	//{
	//	glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat[16]) * 2, 0, GL_STREAM_DRAW);
	//	glBindBufferBase(GL_UNIFORM_BUFFER, 0, VboId[0]);
	//}
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void engine::Scene::draw(float delta)
{
	//mat4 ViewMatrix = camera->ViewMatrix();
	//mat4 projectionMatrix = camera->ProjectionMatrix();

	//const GLsizeiptr matrixSize = sizeof(GLfloat[16]);

	//glBindBuffer(GL_UNIFORM_BUFFER, VboId[0]);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, matrixSize, ViewMatrix.data());
	//glBufferSubData(GL_UNIFORM_BUFFER, matrixSize, matrixSize, projectionMatrix.data());
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);

	root->draw(delta);
}

void engine::Scene::addNode(SceneNode * node)
{
	root->addNode(node);
}

void engine::Scene::updateModel(mat4 trs)
{
	root->updateModel(trs);
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


