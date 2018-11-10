#version 330 core

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexcoord;
out vec3 exNormal;


uniform mat4 ModelMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
	exNormal = inNormal;
}
