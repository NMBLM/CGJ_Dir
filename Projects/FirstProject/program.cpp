#include "program.h"

Program::Program()
{
	
}

Program::Program(GLuint i)
{
	id = i;
}

void Program::attachShader(Shader shader)
{
	glAttachShader(id, shader.id);
}

void Program::detachShader(Shader shader)
{
	glDetachShader(id, shader.id);
	shader.deleteShader();
}

void Program::link()
{
	glLinkProgram(id);
}

void Program::bindAttribLocation(const int atnum, const char * atname)
{

	glBindAttribLocation(id,atnum,atname);

}

GLint Program::UniformId(const char * uniformName)
{

	return glGetUniformLocation(id, uniformName);


}

GLint Program::UniformLocation(const char * uniformName)
{
	return glGetUniformLocation(id, uniformName);
}

GLint Program::uniformBlockIndex(const char * uniformName)
{
	return glGetUniformBlockIndex(id, uniformName);
}
