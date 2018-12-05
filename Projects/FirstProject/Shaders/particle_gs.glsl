#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec4 color;
uniform vec3 position;
uniform mat3 rotation;

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

void build_quad2(vec4 pos)
{    
	//pos = vec4(rotation * position.xyz,1.0f) + pos;
	pos = vec4(((-1) * rotation) * position.xyz,1.0f) + pos;
//	pos = vec4(rotation * pos.xyz,1.0f) + position;
//	pos = pos + position;
//	pos = vec4(rotation * pos.xyz,1.0f);
	

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
	//build_quad2(gl_in[0].gl_Position);

}  