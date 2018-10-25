#include "Shader.h"

VertexShader::VertexShader() {

}

VertexShader::VertexShader(const char * shaderFilename)
{
	std::ifstream v(shaderFilename);
	std::string data;
	data.assign((std::istreambuf_iterator<char>(v)),
		std::istreambuf_iterator<char>());
	const GLchar * VertexShader = data.c_str();

	id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(id, 1, &VertexShader, 0);
	glCompileShader(id);
}

FragmentShader::FragmentShader() {

}

FragmentShader::FragmentShader(const char * shaderFilename)
{
	std::ifstream v(shaderFilename);
	std::string data;
	data.assign((std::istreambuf_iterator<char>(v)),
		std::istreambuf_iterator<char>());
	const GLchar * FragmentShader = data.c_str();

	id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(id, 1, &FragmentShader, 0);
	glCompileShader(id);
}

void Shader::deleteShader()
{
	glDeleteShader(id);
}
