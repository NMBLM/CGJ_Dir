#version 330 core
in vec3 Postion;
in vec2 Texcoord;
in vec3 Normal;

uniform mat4 ModelMatrix;

uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Postion, 1.0);
}