#version 330 core

in vec3 vertex;
in vec3 normal;
in vec2 texCoord;

out vec3 gPosition;
out vec3 gNormal;
out vec4 gAlbedoSpec;


void main()
{	
	gPosition = vertex;	//WorldSpace
	gNormal = normal;
	gAlbedoSpec.rgb = vec3(0.57735f,0.57735f,0.57735f);//General Color
	gAlbedoSpec.a = 0.57735f;//Specular
}