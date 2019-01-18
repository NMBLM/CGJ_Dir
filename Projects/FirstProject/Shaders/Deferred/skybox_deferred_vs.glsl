
#version 330 core
in vec3 Postion;
in vec2 Texcoord;
in vec3 Normal;

out vec3 FragPos;
out vec3 TexCoords;
out vec3 aNormal;

uniform mat4 ModelMatrix;

uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

void main()
{
    vec4 worldPos = ModelMatrix * vec4(Postion, 1.0);
	//World Coordinates
    FragPos = worldPos.xyz; 
	
    mat3 normalMatrix = transpose(inverse(mat3(ModelMatrix)));
    aNormal = normalMatrix * aNormal;
	TexCoords = Postion;

    gl_Position = ProjectionMatrix * ViewMatrix * worldPos;
}