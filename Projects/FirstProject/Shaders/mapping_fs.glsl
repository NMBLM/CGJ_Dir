#version 330 core 

out vec4 out_Color;

in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main(){
//    out_Color = mix(texture(texture0, TexCoord),texture(texture1, TexCoord), 0);
    out_Color = texture(texture0, TexCoord);
}