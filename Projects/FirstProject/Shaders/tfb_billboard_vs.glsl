#version 330                                                                        
                                                                                    
in vec3 Position;                                             
#define SPEEDX 60
#define SPEEDZ -60
#define SCALEX 0.03f
#define SCALEZ 0.02f
void main()                                                                         
{                                                                                   
    gl_Position = vec4(Position, 1.0);  
	gl_Position = vec4(gl_Position.x + SCALEX * cos((gl_Position.y+gl_Position.z)* (SPEEDX)) ,
		gl_Position.y ,  gl_Position.z + SCALEZ * cos((gl_Position.y+gl_Position.x)*(SPEEDZ)),1.0f);
}                                                                                   
