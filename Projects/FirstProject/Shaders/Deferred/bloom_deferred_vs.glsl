#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inTangent;
in vec3 inBiTangent;

uniform mat4 ModelMatrix;
uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

out vec2 TexCoord;
out vec3 PassNormal;
out vec3 WorldPos;
out vec3 Tangent;
out vec3 BiTangent;

void main(void){

    mat4 normalMatrix = transpose(inverse(ViewMatrix * ModelMatrix));
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
    TexCoord = inTexcoord;
    PassNormal = (normalMatrix * vec4(inNormal, 0.0)).xyz;
    WorldPos = (ModelMatrix * vec4(inPosition, 1.0)).xyz;
    Tangent = (ModelMatrix * vec4(inTangent, 0.0)).xyz;
    BiTangent = (ModelMatrix * vec4(inBiTangent, 0.0f)).xyz;
}