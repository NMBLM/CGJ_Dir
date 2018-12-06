#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec3 vertex;

#define POINT 0.1f
void build_quad(vec4 pos)
{    
	mat4 mvp = ProjectionMatrix * ViewMatrix;

	pos = mvp * pos;
	
    gl_Position = pos + vec4(-POINT, -POINT, 0.0, 0.0);   // 1:bottom-left
	vertex = vec3(gl_Position);
    EmitVertex();
	gl_Position = pos + vec4(POINT, -POINT, 0.0, 0.0);   // 2:bottom-right
	vertex = vec3(gl_Position);
    EmitVertex();
    gl_Position = pos + vec4(-POINT, POINT, 0.0, 0.0);   // 3:top-left
	vertex = vec3(gl_Position);
    EmitVertex();
    gl_Position = pos + vec4(POINT, POINT, 0.0, 0.0);   // 4:top-right
	vertex = vec3(gl_Position);
    EmitVertex();
    EndPrimitive();
}


void main() {   
    build_quad(gl_in[0].gl_Position);
}  