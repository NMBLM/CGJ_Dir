#version 330 core
out vec4 gPosition;
out vec4 gNormal;
out vec4 gAlbedoSpec;
out vec4 gBright;

in vec3 PassNormal;
in vec3 WorldPos;

uniform vec4 forcedColor;

void main()
{           
	gPosition = vec4(WorldPos,1.0f);
	gNormal = vec4(normalize(PassNormal),1.0f);
	gAlbedoSpec.rgb = forcedColor.rgb;
	gAlbedoSpec.a = forcedColor.x;
	gBright = vec4(forcedColor.rgb,1.0f);

}