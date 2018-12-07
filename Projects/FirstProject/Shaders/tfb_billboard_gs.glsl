#version 330

layout(points) in;
layout (triangle_strip, max_vertices = 4) out;


uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec3 vertex;
out vec3 normal;
out vec2 texCoord;

#define POINT 0.02f
void build_quad(vec4 pos)
{    
	
	mat4 mvp = ProjectionMatrix * ViewMatrix;
	mat4 ModelMatrix = mat4(1);
	// place eye in model space slash world space(because model matrix is the identity matrix),
	// so this is to bring eye from eye space slash view space;
	mat4 invView = inverse(ViewMatrix * ModelMatrix);
	vec3 eye = vec3(invView[3])/invView[3][3]; // world space
	vec3 E = normalize(eye - pos.xyz);
	vec3 Worldup = vec3(0,1,0);
	vec3 side = normalize(cross(E,Worldup)) ;
	vec3 up = normalize(cross(side,E)) ;

	//normal = normalize(vec3(mvp* vec4(E,1.0f)));
	normal = E;
    gl_Position = mvp * vec4(pos.xyz + side* POINT - up* POINT,1.0f);    // 1:bottom-left
	vertex = gl_Position.xyz;
	texCoord = vec2(0,0);
    EmitVertex();   
    gl_Position = mvp * vec4(pos.xyz - side* POINT - up* POINT,1.0f);    // 2:bottom-right
	vertex = gl_Position.xyz;
	texCoord = vec2(0,0);
    EmitVertex();
    gl_Position = mvp * vec4(pos.xyz + side* POINT + up* POINT,1.0f);    // 3:top-left
	vertex = gl_Position.xyz;
	texCoord = vec2(0,0);
    EmitVertex();
    gl_Position = mvp * vec4(pos.xyz - side* POINT + up* POINT,1.0f);    // 4:top-right
	vertex = gl_Position.xyz;
	texCoord = vec2(0,0);
    EmitVertex();
    EndPrimitive();
}

void build_quad2(vec4 pos)
{    
	mat4 mvp = ProjectionMatrix * ViewMatrix;
	mat4 ModelMatrix = mat4(1);
	// place eye in model space slash world space(because model matrix is the identity matrix),
	// so this is to bring eye from eye space slash view space;
	mat4 invView = inverse(ViewMatrix * ModelMatrix);
	vec3 eye = vec3(invView[3])/invView[3][3]; // world space
	vec3 E = normalize(eye - pos.xyz);
	vec3 Worldup = vec3(0,1,0);
	vec3 side = normalize(cross(E,Worldup)) ;
	vec3 up = normalize(cross(side,E)) ;

	normal = normalize(vec3(mvp* vec4(E,1.0f)));
	//normal = E;
    gl_Position = mvp * vec4(pos.xyz + side* POINT - up* POINT,1.0f);    // 1:bottom-left
	vertex = gl_Position.xyz;
	texCoord = vec2(0,0);
	normal = normalize(E + side - up);
	normal = vec3(mvp * vec4(normal,1));
    EmitVertex();   
    gl_Position = mvp * vec4(pos.xyz - side* POINT - up* POINT,1.0f);    // 2:bottom-right
	vertex = gl_Position.xyz;
	texCoord = vec2(0,0);
	normal = normalize(E - side - up);
	normal = vec3(mvp * vec4(normal,1));
    EmitVertex();
    gl_Position = mvp * vec4(pos.xyz + side* POINT + up* POINT,1.0f);    // 3:top-left
	vertex = gl_Position.xyz;
	texCoord = vec2(0,0);
	normal = normalize(E + side + up);
	normal = vec3(mvp * vec4(normal,1));
    EmitVertex();
    gl_Position = mvp * vec4(pos.xyz - side* POINT + up* POINT,1.0f);    // 4:top-right
	vertex = gl_Position.xyz;
	texCoord = vec2(0,0);
	normal = normalize(E - side + up);
	normal = vec3(mvp * vec4(normal,1));
    EmitVertex();
    EndPrimitive();
}

void main() {

	build_quad(gl_in[0].gl_Position);

}  