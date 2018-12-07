#version 330 core

in vec4 ParticleColor;
in vec3 vertex;

out vec4 out_color;

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

struct Material{
	float shininess;
	vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 3  
uniform PointLight pointLights[NR_POINT_LIGHTS];
Material material;
vec3 eye;

void setupMaterial(){
	material.shininess = 0.5f;
	material.diffuse = normalize(vec3(0.57735f,0.57735f,0.57735f));
    material.specular = normalize(vec3(0.57735f,0.57735f,0.57735f));
}
vec3 someFunctionToCalculatePointLight(PointLight p){
	vec3 E = normalize(eye - vertex);
	vec3 N = normalize(eye - vertex);
	vec3 L = p.position - vertex;
	float dist = length(L);
	L = normalize(L);
	float attenuation = 1/(p.constant + p.linear * dist + p.quadratic * pow(dist,2));
    // diffuse shading
    float diff = max(dot(N, L), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-L, N);
    float spec = pow(max(dot(E, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient  = p.ambient  * material.diffuse;
    vec3 diffuse  = p.diffuse  * diff * material.diffuse;
    vec3 specular = p.specular * spec * material.specular;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);

 
}

void main()
{	
	eye = vec3(ViewMatrix[3])/ViewMatrix[3][3];
	
	setupMaterial();
	// define an output color value
	vec3 op = vec3(0.0f,0.0f,0.0f);
	// do the same for all point lights
	for (int i= 0; i < NR_POINT_LIGHTS; i++){
		op += someFunctionToCalculatePointLight(pointLights[i]);
	}

	out_color = vec4(op, 1.0f);
}