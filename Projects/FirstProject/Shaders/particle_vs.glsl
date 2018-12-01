
#version 330 core
in vec3 inPosition; 

out vec4 ParticleColor;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform vec4 color;
uniform vec3 position;
void main()
{
    float scale = 1.0f;
	vec3 pos =  inPosition + vec3(position);
    ParticleColor = color;
    gl_Position = ProjectionMatrix * ViewMatrix * vec4((pos.xy * scale), 0.0f, 1.0f);
}