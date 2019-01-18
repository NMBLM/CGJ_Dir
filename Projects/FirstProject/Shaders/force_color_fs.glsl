#version 330 core 

in vec3 exNormal;
out vec4 out_Color; 

uniform vec4 forcedColor;


void main(void) 
{ 
	//out_Color = vec4((vec3(forcedColor.x,forcedColor.y,forcedColor.z) + (exNormal)) * 0.5,1);
	if(exNormal.y <0){
		out_Color = (forcedColor + vec4(0.3,0.3,0.3,0)) * vec4(0.7,0.5,0.9,1);
	}else{
		out_Color = forcedColor;
	}
}
