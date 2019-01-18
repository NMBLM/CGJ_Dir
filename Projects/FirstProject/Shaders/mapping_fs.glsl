#version 330 core 

out vec4 outColor;

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

struct DirectionalLight                                                             {                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
    vec3 Direction;                                                                 
};  

uniform sampler2D noodleTex;
uniform sampler2D noodleNormal;
uniform sampler2D noodleSpec;


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

void main(){

    float gMatSpecularIntensity = 1.0f;
    float gSpecularPower = 1.0f;

    DirectionalLight gDirectionalLight =  DirectionalLight(
        vec3(1.0f,1.0f, 1.0f),
        0.2f,
        0.3f,
        normalize(vec3(1.0f, 0.0f, 0.0f))
    );

    mat4 invView = inverse(ViewMatrix);
    vec3 eye = vec3(invView[3])/invView[3][3];

    vec4 AmbientColor = vec4(gDirectionalLight.Color * gDirectionalLight.AmbientIntensity, 1.0f);
    vec3 LightDirection = -gDirectionalLight.Direction;
    
//    vec3 Normal = CalcBumpedNormal();
    vec3 Normal = normalize(PassNormal);
    float DiffuseFactor = dot(Normal, LightDirection);

    vec4 DiffuseColor = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) *
            gDirectionalLight.DiffuseIntensity *
            DiffuseFactor;

        vec3 VertexToEye = normalize(eye - WorldPos);
        vec3 LightReflect = normalize(reflect(gDirectionalLight.Direction, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        if (SpecularFactor > 0) {
            SpecularFactor = pow(SpecularFactor, gSpecularPower);
            SpecularColor = vec4(gDirectionalLight.Color * gMatSpecularIntensity * SpecularFactor, 1.0f)
                          * texture2D(noodleSpec, TexCoord).x;
        }
    }
         outColor = texture2D(noodleTex, TexCoord.xy) * (AmbientColor + DiffuseColor + SpecularColor);
}