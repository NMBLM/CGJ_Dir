#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 Texcoord;

out vec2 TexCoords;

void main()
{
    TexCoords = Texcoord;
    gl_Position = vec4(Position, 1.0);
}
