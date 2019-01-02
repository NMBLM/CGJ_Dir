#version 330 core 

out vec4 out_color;

in vec2 TexCoord;
in vec3 PassNormal;
in vec3 WorldPos;
in vec3 Tangent;
in vec3 BiTangent;

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
#define NR_POINT_LIGHTS 8  
uniform PointLight pointLights[NR_POINT_LIGHTS];


uniform sampler2D noodleTex;
uniform sampler2D noodleNormal;
uniform sampler2D noodleSpec;

vec3 eye;

vec3 CalcBumpedNormal(){
    vec3 Normal = normalize(PassNormal);
    vec3 TangentN = normalize(Tangent);
    TangentN = normalize(TangentN - dot(TangentN, Normal) * Normal);
    vec3 BiTangentN = cross(TangentN, Normal);
    vec3 BumpMapNormal = texture(noodleNormal, TexCoord).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(TangentN, BiTangentN, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

vec3 someFunctionToCalculatePointLight(PointLight p){

	vec3 lightPos = vec3( vec4(p.position,1.0f)); //WorldSpace
	vec3 normal = PassNormal;
	//normal = CalcBumpedNormal();

	//Vertex in WorldSpace
	vec3 E = normalize(eye - WorldPos);
	//vec3 N = normalize(eye - vertex);
	vec3 N = normalize(normal);
	vec3 L =  lightPos - WorldPos;
	float dist = length(L);
	L = normalize(L);
	float attenuation = 1/(p.constant + p.linear * dist + p.quadratic * pow(dist,2));

    // diffuse shading
    float diff = max(dot(N, L), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-L, N);
    float spec = pow(max(dot(E, reflectDir), 0.0), 1.0f);

    // combine results
    vec3 ambient  = p.ambient ;
    vec3 diffuse  = p.diffuse  * diff ;
    vec3 specular = p.specular * spec * texture2D(noodleSpec, TexCoord).x; ;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);

 
}

void main()
{	
	mat4 invView = inverse(ViewMatrix);
	eye = vec3(invView[3])/invView[3][3]; // world space
	// define an output color value
	vec3 op = vec3(0.0f,0.0f,0.0f);
	// do the same for all point lights
	for (int i= 0; i < NR_POINT_LIGHTS; i++){
		op += someFunctionToCalculatePointLight(pointLights[i]);
	}

	out_color = texture2D(noodleTex, TexCoord.xy) *vec4(op, 1.0f);
}