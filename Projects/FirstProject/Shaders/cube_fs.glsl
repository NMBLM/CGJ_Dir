#version 330 core 

in vec3 exNormal;
out vec4 out_Color; 

uniform float k;


void main(void) 
{ 
	out_Color = vec4((vec3(1,1,1) - (exNormal* cos(k))) * 0.5,1); 
} 
