#version 330 core

in vec3 vertex;
in vec3 normalViewSpace;
in vec3 normal;
in vec2 texCoord;

uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

out vec4 gPosition;
out vec4 gNormal;
out vec4 gAlbedoSpec;
out vec4 gBright;
out vec4 gPositionViewSpace;
out vec4 gNormalViewSpace;

void main()
{	
	gPosition = vec4(vertex,1.0f);	//WorldSpace
	gNormal = vec4(normalize(normal),1.0f);
	gAlbedoSpec.rgb = vec3(0.57735f,0.57735f,0.57735f);//General Color
	gAlbedoSpec.a = 0.57735f;//Specular
	gBright = vec4(0.0f);
	gPositionViewSpace = ViewMatrix * vec4(vertex,1.0f);
	gNormalViewSpace = vec4(normalize(normalViewSpace),1.0f);

}