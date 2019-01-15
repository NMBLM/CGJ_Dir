#version 330 core
out vec4 FragColor;
out vec4 BrightColor;

in vec3 vertex;
in vec3 normal;
in vec2 texCoord;

uniform vec4 forcedColor;

void main()
{           
    FragColor = forcedColor;
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0){
        BrightColor = vec4(FragColor.rgb, 1.0);
	}else{
		BrightColor = vec4(0.0f,0.0f,0.0f,1.0f);
	}
}