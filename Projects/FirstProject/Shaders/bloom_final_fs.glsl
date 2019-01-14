#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

void main()
{           
	float exposure = 1.0f;
    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
	hdrColor += bloomColor; // additive blending
	//hdrColor += 2*bloomColor; // additive blending
	//hdrColor *= bloomColor; // Mult blending
	//hdrColor = bloomColor; // blur/brightparts only
	//hdrColor = hdrColor; // no blending
	//hdrColor /= bloomColor; // div blending
	//hdrColor = bloomColor/hdrColor; // div2 blending

    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}