#version 330 core

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexcoord;
out vec4 forceColor;


uniform vec4 forcedColor;
uniform mat4 ModelMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
	if(inNormal.y <0){
		forceColor = (forcedColor + vec4(0.3,0.3,0.3,0)) * vec4(0.7,0.5,0.9,1);
	}else{
		forceColor = forcedColor;
	}
}
