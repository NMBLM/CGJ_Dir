#version 330 core 

in vec3 exNormal;
out vec4 out_Color; 



void main(void) 
{ 
	if(exNormal.x <0 || exNormal.y <0 || exNormal.z <0){
		out_Color = vec4(vec3(1,1,1) + exNormal, 1);
	}else{
		out_Color = vec4(exNormal.x,exNormal.y,exNormal.z,1); 
	}
} 
