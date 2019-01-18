#version 330 core 
in vec3 TexCoord;
in vec3 PassNormal;
in vec3 WorldPos;

uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

out vec4 gPosition;
out vec4 gNormal;
out vec4 gAlbedoSpec;
out vec4 gBright;

uniform samplerCube skybox;

void main()
{	
	gPosition = ViewMatrix * vec4(WorldPos,1.0f);
	gNormal = vec4(normalize(PassNormal),1.0f);
	gAlbedoSpec.rgb = texture(skybox,TexCoord).rgb;
	gAlbedoSpec.a = texture(skybox,TexCoord).x;
	gBright = vec4(-1.0f);
}