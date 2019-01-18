#version 330 core
out vec4 gPosition;
out vec4 gNormal;
out vec4 gAlbedoSpec;
out vec4 gBright;

out vec3 TexCoords;
out vec3 PassNormal;
out vec3 WorldPos;

uniform samplerCube skybox;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
	// World Coordinates
    gPosition = vec4(WorldPos,1.0f);

    // also store the per-fragment normals into the gbuffer
    gNormal = vec4(normalize(PassNormal),1.0f);

    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(skybox, TexCoords).rgb;

    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(skybox, TexCoords).x;

	gBright = vec4(0.0f);


}
