#version 330 core
out vec3 gPosition;
out vec3 gNormal;
out vec4 gAlbedoSpec;

in vec3 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform samplerCube skybox;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
	// World Coordinates
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(skybox, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(skybox, TexCoords).r;

}