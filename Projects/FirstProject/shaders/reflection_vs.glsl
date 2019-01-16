#version 330 core

in vec3 Position;
in vec2 Texcoords;

uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec2 TexCoord;


void main(void){
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position,1.0f);
	TexCoord = Texcoords;
}