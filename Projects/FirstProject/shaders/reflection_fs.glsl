#version 330 core 

in vec2 TexCoord;
uniform sampler2D reflection;


out vec4 out_Color; 
out vec4 bright_color;

void main(void) { 
	out_Color = texture2D(reflection, TexCoord) * vec4(0.1f); 
	bright_color = vec4(0.0, 0.0, 0.0, 1.0);
} 
