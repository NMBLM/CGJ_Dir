#version 330 core
out vec4 FragColor;
out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gBright;

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
    
    // then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.1f; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_POINT_LIGHTS; ++i)
    {
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
	//Normal
	if(mode < 0.9f){
		FragColor = vec4(lighting, 1.0);
		vec4 bright = texture(gBright, TexCoords).rgba;
		if(bright.r > 0.0f || bright.g > 0.0f || bright.b > 0.0f){
			BrightColor = bright;
		}else{
			float brightness = dot(FragColor.xyz, vec3(0.2126, 0.7152, 0.0722));
			if(brightness > 1.0){
				BrightColor = FragColor;
			}else{
				BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
			}
		}
	}else // Position
	if(mode > 0.9f && mode < 1.9f){
		FragColor = vec4(texture(gPosition, TexCoords).rgb,1.0f);
		BrightColor = vec4(0.0f);
	}else // Normals
	if(mode > 1.9f && mode < 2.9f){
		FragColor = vec4(texture(gNormal, TexCoords).rgb,1.0f);
		BrightColor = vec4(0.0f);
	}else // Basic Color
	if(mode > 2.9f && mode < 3.9f){
		FragColor = texture(gAlbedoSpec, TexCoords).rgba;
		BrightColor = vec4(0.0f);
	}else // Bright Ness Unblurred
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
	}else // Bright Ness Blurred
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
	}
	//This way we can force anything that has gBright to be bloomed


}