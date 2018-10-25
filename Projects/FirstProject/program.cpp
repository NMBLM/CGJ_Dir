#include "program.h"

Program::Program()
{
	
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
