#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec3 rotation;
uniform vec4 color;
uniform vec3 position;
out vec4 ParticleColor;

void build_quad(vec4 pos)
{    
	pos = pos + position;
    gl_Position = pos + vec4(-0.01, -0.01, 0.0, 0.0);    // 1:bottom-left
    EmitVertex();   
    gl_Position = pos + vec4( 0.01, -0.01, 0.0, 0.0);    // 2:bottom-right
    EmitVertex();
    gl_Position = pos + vec4(-0.01,  0.01, 0.0, 0.0);    // 3:top-left
    EmitVertex();
    gl_Position = pos + vec4( 0.01,  0.01, 0.0, 0.0);    // 4:top-right
    EmitVertex();
    EndPrimitive();
}

void main() {   
	ParticleColor = color;
    build_quad(gl_in[0].gl_Position);
	//gl_Position = gl_in[0].gl_Position;
	//EmitVertex();   
}  