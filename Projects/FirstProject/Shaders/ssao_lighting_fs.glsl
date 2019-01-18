#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D ssao;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 
#define NR_POINT_LIGHTS 11
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 viewPos;

uniform float mode;

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    float AmbientOcclusion = texture(ssao, TexCoords).r;
    
    // then calculate lighting as usual
    vec3 lighting  = vec3(0.3 * Diffuse * AmbientOcclusion);
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_POINT_LIGHTS; ++i){
        // diffuse
        vec3 lightDir = normalize(pointLights[i].position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * pointLights[i].diffuse;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = pointLights[i].diffuse * spec * Specular;
        // attenuation
        float distance = length(pointLights[i].position - FragPos);
        float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;        
    }
    if(mode < 0.9f){
        FragColor = vec4(lighting, 1.0);
    }else if(mode > 0.9f && mode < 1.9f){
        FragColor = vec4(texture(gPosition, TexCoords).rgb,1.0f);
    }else if(mode > 1.9f && mode < 2.9f){
        FragColor = vec4(texture(gNormal, TexCoords).rgb,1.0f);
    }else if(mode > 2.9f && mode < 3.9f){
        FragColor = texture(gAlbedoSpec, TexCoords).rgba;
    }



}