#version 330 core

in vec3 inPosition;

uniform mat4 ModelMatrix;

uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

out vec3 TexCoords;

void main(void){
	TexCoords = inPosition;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
    
}