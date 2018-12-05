#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec4 color;
uniform vec3 position;
uniform mat3 rotation;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec4 ParticleColor;

vec3 eye = normalize(vec3(ViewMatrix[3][0],ViewMatrix[3][1],-ViewMatrix[3][2]));
vec3 up = vec3(0,1,0);
vec3 side = cross(eye,up);

void build_quad(vec4 pos)
{    
	//gl_Position = ProjectionMatrix * ViewMatrix  * vec4(inPosition.xy,0.0f, 1.0f);
	pos = pos + position;
	side = side * 0.01;
    gl_Position = ProjectionMatrix * ViewMatrix  * (pos + side - up*0.01 );    // 1:bottom-left
    EmitVertex();   
    gl_Position = ProjectionMatrix * ViewMatrix  * (pos - side - up*0.01);    // 2:bottom-right
    EmitVertex();
    gl_Position = ProjectionMatrix * ViewMatrix  * (pos + side + up*0.01 );    // 3:top-left
    EmitVertex();
    gl_Position = ProjectionMatrix * ViewMatrix  * (pos - side + up*0.01);    // 4:top-right
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