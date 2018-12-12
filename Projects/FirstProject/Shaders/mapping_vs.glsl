#version 330 core

in vec3 inPosition;
in vec2 inTexcoords;

uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec2 TexCoord;


void main(void){
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
	TexCoord = inTexcoords;
}