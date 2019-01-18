#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D gSSAOBlur;
uniform sampler2D gSSAO;
uniform float mode;
void main(){
    
    if(mode < 0.9f){
        FragColor = vec4(vec3(texture(gSSAO,TexCoords).r), 1.0f);
    }else if(mode > 0.9f && mode < 1.9f){
        FragColor = vec4(vec3(texture(gSSAOBlur,TexCoords).r), 1.0f);
    }else if(mode > 1.9f && mode < 2.9f){
        FragColor =vec4(vec3(texture(gSSAO,TexCoords).r), 1.0f);
    }else if(mode > 2.9f && mode < 3.9f){
        FragColor = vec4(vec3(texture(gSSAOBlur,TexCoords).r), 1.0f);
    }
//    FragColor = vec4(TexCoords, 0.0f ,1.0f);
    
}