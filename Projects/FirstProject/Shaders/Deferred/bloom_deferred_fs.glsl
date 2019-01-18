#version 330 core 
in vec2 TexCoord;
in vec3 PassNormal;
in vec3 PassNormalViewSpace;

in vec3 WorldPos;
in vec3 Tangent;
in vec3 BiTangent;

uniform SharedMatrices
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

uniform sampler2D noodleTex;
uniform sampler2D noodleNormal;
uniform sampler2D noodleSpec;

out vec4 gPosition;
out vec4 gNormal;
out vec4 gAlbedoSpec;
out vec4 gBright;
out vec4 gPositionViewSpace;
out vec4 gNormalViewSpace;

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
vec3 CalcBumpedNormalView(){
    vec3 Normal = normalize(PassNormalViewSpace);
    vec3 TangentN = normalize((ViewMatrix * vec4(Tangent,1.0f)).xyz);
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


void main()
{	
	gPosition = vec4(WorldPos,1.0f);
	gNormal = vec4(CalcBumpedNormal(),1.0f);
	gAlbedoSpec.rgb = texture(noodleTex,TexCoord).rgb;
	gAlbedoSpec.a = texture(noodleSpec,TexCoord).x;
	gBright = vec4(0.0f);
	gPositionViewSpace = ViewMatrix * vec4(WorldPos,1.0f);
	gNormalViewSpace = vec4(CalcBumpedNormalView(),1.0f);

}