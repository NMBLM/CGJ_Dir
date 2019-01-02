#version 330 core
in vec3 Position;
in vec2 Texcoords;

out vec2 TexCoords;

void main()
{
    TexCoords = Texcoords;
    gl_Position = vec4(Position, 1.0);
}