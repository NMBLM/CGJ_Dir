#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;


uniform mat4 ModelMatrix;
uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

out vec3 TexCoord;
out vec3 PassNormal;
out vec3 WorldPos;

void main(void){
    mat4 normalMatrix = transpose(inverse(ModelMatrix));
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
    PassNormal = (normalMatrix * vec4(inNormal, 0.0)).xyz;
    WorldPos = (ModelMatrix * vec4(inPosition, 1.0)).xyz;
	TexCoord = WorldPos;

}