#version 330 core
out vec4 FragColor;

uniform vec3 forcedColor;

void main()
{           
    FragColor = vec4(forcedColor, 1.0);
}