#include "Shader.h"

Shader::Shader()
{
}

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	std::ifstream v(vertexShaderFile);
	std::string data;
	data.assign((std::istreambuf_iterator<char>(v)),
		std::istreambuf_iterator<char>());
	const GLchar * VertexShader = data.c_str();


	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	std::ifstream f(fragmentShaderFile);
	data.assign((std::istreambuf_iterator<char>(f)),
		std::istreambuf_iterator<char>());
	const GLchar * FragmentShader = data.c_str();

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, VERTICES, "in_Position");
	glBindAttribLocation(ProgramId, COLORS, "in_Color");

	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");
	UniformColorId = glGetUniformLocation(ProgramId, "force_color");

	glDetachShader(ProgramId, VertexShaderId);
	glDeleteShader(VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);
	glDeleteShader(FragmentShaderId);

}

void Shader::useProgram()
{
}

void use() {

}

