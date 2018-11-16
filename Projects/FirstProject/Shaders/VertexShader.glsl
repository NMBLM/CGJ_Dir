#version 330 core

in vec4 in_Position;

uniform mat4 ModelMatrix;

void main(void)
{
	gl_Position = ModelMatrix * in_Position;
}
