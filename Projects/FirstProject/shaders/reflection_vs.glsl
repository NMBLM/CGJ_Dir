#version 330 core

in vec3 Position;
in vec2 Texcoords;
in vec3 Normal;

uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec3 aPosition;
out vec2 TexCoord;
out vec3 aNormal;

void main(void){
    aNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position,1.0f);
	TexCoord = Texcoords;
	aPosition = vec3(ModelMatrix * vec4(Position, 1.0));

}

