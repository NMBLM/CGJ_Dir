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

	root->draw(delta,MatrixFactory::createIdentityMatrix4());
}

void engine::Scene::addNode(SceneNode * node)
{
	root->addNode(node);
}

void engine::Scene::updateModel(mat4 trs)
{
	root->updateModel(trs);
}

void engine::Scene::resetAnimator()
{
	root->resetAnimator();
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

void engine::SceneNode::draw(float delta,mat4 fm)
{
	mat4 m = fm * model;
	if (mesh != nullptr) {
		if (anime != nullptr) {
			anime->update(delta);
			//m =   model *  inverse(model) * anime->calcAnimation(model) * model;
			m = fm * anime->calcAnimation(model);// *model;
			//m =  (inverse(model) * anime->calcAnimation(model) * model) * model;

		}
		if (!fcmp(color.x ,-1)) {
			mesh->draw(m, this->getShaderProgram(),color);
		}
		else {
			mesh->draw(m, this->getShaderProgram());
		}
	}
	for (auto& s : nodes) {
		s->draw(delta, m);
	}
}

void engine::SceneNode::updateModel(mat4 trs)
{
	model = trs * model;
	if (anime != nullptr) {
		anime->lastMatrix = trs * anime->lastMatrix;
	}
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

void engine::SceneNode::setColor(vec4 c)
{
	color.x = c.x;
	color.y = c.y;
	color.z = c.z;
	color.w = c.w;
}

void engine::SceneNode::addAnimator(Animator* a)
{
	a->lastMatrix = model;
	anime = a;
}

Animator* engine::SceneNode::getAnimator()
{
	return anime;
}

void engine::SceneNode::resetAnimator()
{
	if (anime != nullptr) {
		anime->reset();
	}
	for (auto& s : nodes) {
		s->resetAnimator();
	}
}

ShaderProgram * engine::SceneNode::getShaderProgram() {
	if (shaderProgram != nullptr) {
		return shaderProgram;
	}
	std::cout << "CRASH AND BURN on getShaderProgram" << std::endl;
	exit(1203124);
}


