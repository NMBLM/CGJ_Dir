#include "shape.h"
#include "shader.h"
#include "program.h"
#include "vector.h"

using namespace engine;


void Shape::createBuffers(const Vertex* v, const GLubyte* i)
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(3, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{
			glBufferData(GL_ARRAY_BUFFER, vSize, v, GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
		{
			glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat[16]) * 2, 0, GL_STREAM_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, VboId[2]);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[2]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, i, GL_STATIC_DRAW);
		}
		
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void Shape::destroyBuffers()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(0);
}



Triangle::Triangle()
{
	const Vertex v[] =
	{
		{{ 0.0f, 0.0f, 0.0f, 1.0f }},
		{{ 0.8f, 0.0f, 0.0f, 1.0f }},
		{{ 0.4f, 0.4f, 0.0f, 1.0f }}
	};

	const GLubyte i[] =
	{
		0,1,2,2,1,0
	};


	vSize = sizeof(v);
	iSize = sizeof(i);
	createBuffers(v,i);

}

void Triangle::draw(engine::mat4 transform, const  vec4  color, Program prog)
{
	glBindVertexArray(getVaoId());
	glUseProgram(prog.id);

	glUniform4fv(prog.UniformId("force_color"), 1, color.data());
	glUniformMatrix4fv(prog.UniformId("ModelMatrix"), 1, GL_FALSE, transform.data());
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);
	vec4 colorL = color * 0.80f;
	glUniform4fv(prog.UniformId("force_color"), 1, colorL.data());
	glDrawArrays(GL_TRIANGLES, 3, 3);



	glUseProgram(0);
	glBindVertexArray(0);
}

void Triangle::draw(mat4 transform, mat4 view, mat4 proj, const vec4 color, Program prog)
{
	GLsizeiptr matrixSize = sizeof(GLfloat[16]);

 	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, matrixSize, view.data());
	glBufferSubData(GL_UNIFORM_BUFFER, matrixSize, matrixSize, proj.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	draw(transform, color, prog);
}

Square::Square()
{
	const Vertex v[] =
	{
		{{  0.0f,  0.0f,  0.0f, 1.0f }  }, //middle left
		{{  0.2f,  -0.2f,  0.0f, 1.0f } }, //bottom middle
		{{  0.4f,  0.0f, 0.0f, 1.0f }   }, //middle right
		{{  0.2f,  0.2f, 0.0f, 1.0f }   }, //top middle
	};

	const GLubyte i[] =
	{
		0,1,2,2,3,0,0,3,2,2,1,0
	};


	vSize = sizeof(v);
	iSize = sizeof(i);
	createBuffers(v, i);
}

void Square::draw(engine::mat4 transform, const  vec4  color, Program prog)
{
	glBindVertexArray(getVaoId());
	glUseProgram(prog.id);

	glUniform4fv(prog.UniformId("force_color"), 1, color.data());
	glUniformMatrix4fv(prog.UniformId("ModelMatrix"), 1, GL_FALSE, transform.data());
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (GLvoid*)0);
	//vec4 colorL = color * 0.80f;
	//glUniform4fv(shader.UniformId("force_color"), 1, colorL.data());
	//glDrawArrays(GL_TRIANGLES, 6, 6);

	glUseProgram(0);
	glBindVertexArray(0);
}

void Square::draw(mat4 transform, mat4 view, mat4 proj, const vec4 color, Program prog)
{
	GLsizeiptr matrixSize = sizeof(GLfloat[16]);

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(matrixSize), view.data());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(matrixSize), sizeof(matrixSize), proj.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	draw(transform, color, prog);
}


Parallelogram::Parallelogram()
{
	const Vertex v[] =
	{
		{{  0.0f,  0.0f, 0.0f, 1.0f }}, //bottom left
		{{  0.4f,  0.0f, 0.0f, 1.0f }}, //bottom right
		{{  0.6f,  0.2f, 0.0f, 1.0f }}, //top right
		{{  0.2f,  0.2f, 0.0f, 1.0f }}, //top left
	};

	const GLubyte i[] =
	{
		0,1,3,1,2,3,3,2,1,3,1,0
	};


	vSize = sizeof(v);
	iSize = sizeof(i);
	createBuffers(v, i);
}

void Parallelogram::draw(engine::mat4 transform, const  vec4  color, Program prog)
{
	glBindVertexArray(getVaoId());
	glUseProgram(prog.id);

	glUniform4fv(prog.UniformId("force_color"), 1, color.data());
	glUniformMatrix4fv(prog.UniformId("ModelMatrix"), 1, GL_FALSE, transform.data());
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (GLvoid*)0);
	//vec4 colorL = color * 0.80f;
	//glUniform4fv(shader.UniformId("force_color"), 1, colorL.data());
	//glDrawArrays(GL_TRIANGLES, 6, 6);
	glUseProgram(0);
	glBindVertexArray(0);
}

void Parallelogram::draw(mat4 transform, mat4 view, mat4 proj, const vec4 color, Program prog)
{
	GLsizeiptr matrixSize = sizeof(GLfloat[16]);

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(matrixSize), view.data());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(matrixSize), sizeof(matrixSize), proj.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	draw(transform, color, prog);
}


