#version 330 core
out vec4 gPosition;
out vec4 gNormal;
out vec4 gAlbedoSpec;
out vec4 gBright;

in vec3 PassNormal;
in vec3 WorldPos;

uniform vec4 forcedColor;

uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

#define STRENGTH 0.2f

void main()
{           
	gPosition = ViewMatrix * vec4(WorldPos,1.0f);
	gNormal = vec4(normalize(PassNormal),1.0f);
	gAlbedoSpec.rgb = forcedColor.rgb * STRENGTH;
	gAlbedoSpec.a = forcedColor.x ;
	gBright = vec4(gAlbedoSpec.rgb,1.0f);
	// multiply the color by STRENGTH so its still bright when calculating lightning
	// but the sphere color is not as bright making it a bit better to look at

}