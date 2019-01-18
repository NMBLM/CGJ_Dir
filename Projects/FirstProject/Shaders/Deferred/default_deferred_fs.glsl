#version 330 core
out vec4 FragColor;
out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gBright;
uniform sampler2D gSSAO;
uniform sampler2D gSSAOBlur;

uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

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


uniform float mode;

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    mat4 invView = inverse(ViewMatrix);
    vec3 viewPos = vec3(invView[3])/invView[3][3]; // in WorldSpace
    viewPos = (ViewMatrix * vec4(viewPos,1.0f)).xyz; // now in ViewSpace

    // then calculate lighting as usual
    vec3 lighting  = vec3(0.3f * Diffuse * texture(gSSAOBlur , TexCoords).r);
    if(mode < 0.9f ){ //Blurred Occlusion
        lighting  = vec3( 0.3f * Diffuse * texture(gSSAOBlur , TexCoords).r);
    }else // No Occlusion
    if( mode > 9.9f && mode < 10.9f){
        lighting  = Diffuse * 0.3f;
    }else //Unblurred Occlusion
    if( mode > 10.9f && mode < 11.9f){
        lighting  = vec3( 0.3f * Diffuse * texture(gSSAO , TexCoords).r);
    }
    vec3 viewDir  = normalize(viewPos - FragPos);
    vec4 bright = texture(gBright, TexCoords).rgba;
    if(!(bright.r < 0.0f || bright.g < 0.0f || bright.b < 0.0f)){
        for(int i = 0; i < NR_POINT_LIGHTS; ++i)
        {
            // diffuse
            vec3 lightPos = (ViewMatrix * vec4(pointLights[i].position,1.0f)).xyz;//place in view space
            vec3 lightDir = normalize(lightPos - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * pointLights[i].diffuse;
            // specular
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            vec3 specular = pointLights[i].diffuse * spec * Specular;
            // attenuation
            float distance = length(lightPos - FragPos);
            float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;        
        }
    }else{
        FragColor = texture(gAlbedoSpec, TexCoords).rgba;
    }

    //Normal Mode
    FragColor = vec4(lighting, 1.0);
    if(mode < 0.9f || (mode > 9.9f && mode < 10.9f) || (mode > 10.9f && mode < 11.9f)){
        if(bright.r > 0.0f || bright.g > 0.0f || bright.b > 0.0f){
            FragColor = bright;
            BrightColor = bright;
        }else{
            float brightness = dot(FragColor.xyz, vec3(0.2126, 0.7152, 0.0722));
            if(brightness > 1.0){
                BrightColor = FragColor;
            }else{
                BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
            }
        }
    }else // Position in ViewSpace the one used for calculating the lights
    if(mode > 0.9f && mode < 1.9f){
        FragColor = vec4(texture(gPosition, TexCoords).rgb,1.0f);
        BrightColor = vec4(0.0f);
    }else // Normals in ViewSpace the one used for calculating the lights
    if(mode > 1.9f && mode < 2.9f){
        FragColor = vec4(texture(gNormal, TexCoords).rgb,1.0f);
        BrightColor = vec4(0.0f);
    }else // Basic Color
    if(mode > 2.9f && mode < 3.9f){
        FragColor = texture(gAlbedoSpec, TexCoords).rgba;
        BrightColor = vec4(0.0f);
    }else // Brightness Unblurred
    if(mode > 3.9f && mode < 4.9f){
        FragColor = vec4(lighting, 1.0);
        vec4 bright = texture(gBright, TexCoords).rgba;
        if(bright.r > 0.0f || bright.g > 0.0f || bright.b > 0.0f){
            FragColor = bright;
        }else{
            float brightness = dot(FragColor.xyz, vec3(0.2126, 0.7152, 0.0722));
            if(brightness > 1.0){
                FragColor = FragColor;
            }else{
                FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            }
        }
        BrightColor = vec4(0.0f);
    }else // Brightness Blurred
    if(mode > 4.9f && mode < 5.9f){
        FragColor = vec4(lighting, 1.0);
        vec4 bright = texture(gBright, TexCoords).rgba;
        if(bright.r > 0.0f || bright.g > 0.0f || bright.b > 0.0f){
            FragColor = bright;
            BrightColor  = FragColor;
        }else{
            float brightness = dot(FragColor.xyz, vec3(0.2126, 0.7152, 0.0722));
            if(brightness > 1.0){
                FragColor = FragColor;
                BrightColor  = FragColor;
            }else{
                FragColor = vec4(0.0, 0.0, 0.0, 1.0);
                BrightColor = vec4(0.0f);
            }
        }
    }else // SSAO
    if(mode > 5.9f && mode < 6.9f){
        FragColor = vec4(vec3(texture(gSSAO, TexCoords).r),1.0f);
        BrightColor = vec4(0.0f);
    }else // SSAOBlur
    if(mode > 6.9f && mode < 7.9f){
        FragColor = vec4(vec3(texture(gSSAOBlur, TexCoords).r),1.0f);
        BrightColor = vec4(0.0f);
    }else // Position in WorldSpace for being cool
    if(mode > 7.9f && mode < 8.9f){
        FragColor = inverse(ViewMatrix) * vec4(texture(gPosition, TexCoords).rgb,1.0f);
        BrightColor = vec4(0.0f);
    }else // Normals in WorldSpace for being cool
    if(mode > 8.9f && mode < 9.9f){
        FragColor = inverse(transpose(inverse(ViewMatrix))) * vec4(texture(gNormal, TexCoords).rgb,1.0f);
        BrightColor = vec4(0.0f);
    }
    //This way we can force anything that has gBright to be bloomed


}