#version 330 core
out vec4 gPosition;
out vec4 gNormal;
out vec4 gAlbedoSpec;
out vec4 gBright;
out vec4 gPositionViewSpace;
out vec4 gNormalViewSpace;

in vec3 PassNormal;
in vec3 PassNormalViewSpace;
in vec3 WorldPos;

uniform vec4 forcedColor;

uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

void main()
{           
	gPosition = vec4(WorldPos,1.0f);
	gNormal = vec4(normalize(PassNormal),1.0f);
	gAlbedoSpec.rgb = forcedColor.rgb;
	gAlbedoSpec.a = forcedColor.x;
	gBright = vec4(forcedColor.rgb,1.0f);
	gPositionViewSpace = ViewMatrix * vec4(WorldPos,1.0f);
	gNormalViewSpace = vec4(normalize(PassNormalViewSpace),1.0f);

}