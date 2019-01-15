#version 330 core 

out vec4 out_color;
out vec4 bright_color;

in vec3 TexCoords;

uniform samplerCube skybox;


void main()
{	
	out_color = texture(skybox, TexCoords);

	float brightness = dot(out_color.xyz, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0){
        bright_color = out_color;
    }else{
        bright_color = vec4(0.0, 0.0, 0.0, 1.0);
	}
}