#version 330 core 

out vec4 out_Color;

in vec2 TexCoord;

uniform sampler2D texture0;
uniform vec4 forcedColor;

void main()
{
    out_Color = mix(texture(texture0, TexCoord),forcedColor, 0.3);
}