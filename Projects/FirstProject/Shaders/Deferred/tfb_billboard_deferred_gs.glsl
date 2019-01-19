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

#define POINT 0.008f
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

    normal = normalize((transpose(inverse(ViewMatrix)) * vec4(E,1.0f)).xyz);
    vertex = pos.xyz + side* POINT - up* POINT;
    gl_Position = mvp * vec4(vertex,1.0f);    // 1:bottom-right
    vertex = (ViewMatrix * vec4(vertex,1.0f)).xyz;
    texCoord = vec2(1,0);
    EmitVertex();   
    vertex = pos.xyz - side* POINT - up* POINT;
    gl_Position = mvp * vec4(vertex,1.0f);    // 2:bottom-left
    vertex = (ViewMatrix * vec4(vertex,1.0f)).xyz;
    texCoord = vec2(0,0);
    EmitVertex();
    vertex = pos.xyz + side* POINT + up* POINT;
    gl_Position = mvp * vec4(vertex,1.0f);    // 3:top-right
    vertex = (ViewMatrix * vec4(vertex,1.0f)).xyz;
    texCoord = vec2(1,1);
    EmitVertex();
    vertex = pos.xyz - side* POINT + up* POINT;
    gl_Position = mvp * vec4(vertex,1.0f);    // 4:top-left
    vertex = (ViewMatrix * vec4(vertex,1.0f)).xyz;
    texCoord = vec2(0,1);
    EmitVertex();
    EndPrimitive();
}


void main() {

    build_quad(gl_in[0].gl_Position);

}  