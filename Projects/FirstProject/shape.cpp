#include "shape.h"
#include "shape.h"


void Shape::createBuffers(const Vertex* v, const GLubyte* i)
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{
			glBufferData(GL_ARRAY_BUFFER, vSize, v, GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, i, GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
		0,1,2
	};


	vSize = sizeof(v);
	iSize = sizeof(i);
	createBuffers(v,i);

}

void Triangle::draw(engine::mat4 transform, const  GLfloat * color, Shader shader)
{
	glBindVertexArray(getVaoId());
	glUseProgram(shader.ProgramId);

	glUniform4fv(shader.UniformId("force_color"), 1, color);
	glUniformMatrix4fv(shader.UniformId("Matrix"), 1, GL_FALSE, transform.data());
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUseProgram(0);
	glBindVertexArray(0);
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
		0,1,2,2,3,0
	};


	vSize = sizeof(v);
	iSize = sizeof(i);
	createBuffers(v, i);
}

void Square::draw(engine::mat4 transform, const GLfloat * color, Shader shader)
{
	glBindVertexArray(getVaoId());
	glUseProgram(shader.ProgramId);

	glUniform4fv(shader.UniformId("force_color"), 1, color);
	glUniformMatrix4fv(shader.UniformId("Matrix"), 1, GL_FALSE, transform.data());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUseProgram(0);
	glBindVertexArray(0);
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
		0,1,3,1,2,3
	};


	vSize = sizeof(v);
	iSize = sizeof(i);
	createBuffers(v, i);
}


void Parallelogram::draw(engine::mat4 transform, const GLfloat * color, Shader shader)
{
	glBindVertexArray(getVaoId());
	glUseProgram(shader.ProgramId);

	glUniform4fv(shader.UniformId("force_color"), 1, color);
	glUniformMatrix4fv(shader.UniformId("Matrix"), 1, GL_FALSE, transform.data());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUseProgram(0);
	glBindVertexArray(0);
}