#version 330 core 

in vec3 aPosition;
in vec2 TexCoord;
in vec3 aNormal;
uniform sampler2D reflection;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec4 out_Color; 
out vec4 bright_color;

void main(void) {
	
	mat4 invView = inverse(ViewMatrix);
	vec3 eye = vec3(invView[3])/invView[3][3];

    vec3 I = normalize(aPosition - eye);
    vec3 R = reflect(I, normalize(vec3(0,1,0)));

	out_Color =	vec4(texture2D(reflection, TexCoord).rgb, 1.0);

	bright_color = vec4(0.0, 0.0, 0.0, 1.0);
} 
