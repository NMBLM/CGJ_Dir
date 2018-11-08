#include "shader.h"

VertexShader::VertexShader() {

}

VertexShader::VertexShader(const char * shaderFilename)
{
	std::ifstream v(shaderFilename);
	std::string data;
	data.assign((std::istreambuf_iterator<char>(v)),
		std::istreambuf_iterator<char>());
	const GLchar * VertexShader = data.c_str();
	int success = 0;
	char log[512];
	id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(id, 1, &VertexShader, 0);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
	};

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

	int success = 0;
	char log[512];
	id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(id, 1, &FragmentShader, 0);
	glCompileShader(id);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << log << std::endl;
	};
}

void Shader::deleteShader()
{
	glDeleteShader(id);
}
