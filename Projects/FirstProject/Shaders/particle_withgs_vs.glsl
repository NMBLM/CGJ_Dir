#version 330 core
in vec3 inPosition; 

out vec4 ParticleColor;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform vec4 color;
void main()
{
    float scale = 1.0f;
    ParticleColor = color;
    gl_Position = ProjectionMatrix * ViewMatrix  * vec4(inPosition.xyz, 1.0f);
}