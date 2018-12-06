#version 330 core
in vec3 inPosition; 

out vec4 ParticleColor;
out vec4 exNormal;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};
uniform mat3 rotation;
uniform vec4 color;

void main()
{
    float scale = 1.0f;
    ParticleColor = color;
    //gl_Position = ProjectionMatrix * ViewMatrix  * vec4(inPosition.xyz, 1.0f);
	//gl_Position = ProjectionMatrix * ViewMatrix  * vec4(inPosition.xy,0.0f, 1.0f);
	//gl_Position = vec4(inPosition.xy,0.0f, 1.0f);
	gl_Position = vec4(inPosition.xyz, 1.0f);
	//gl_Position = ProjectionMatrix * ViewMatrix  * vec4(rotation * inPosition.xyz, 1.0f);
}