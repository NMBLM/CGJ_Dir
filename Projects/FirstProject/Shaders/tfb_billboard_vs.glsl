#version 330                                                                        
                                                                                    
in vec3 Position;                                             
#define SPEEDX 60
#define SPEEDZ -60
#define SCALEX 0.06f
#define SCALEZ 0.024f
uniform float rnd1;
uniform float rnd2;
uniform float rnd3;
float random (vec2 st) {
    return fract(sin(dot(st.xy,
        vec2(12.9898,78.233)))*
        43758.5453123);
}
void main()                                                                         
{                                                                                   
    gl_Position = vec4(Position, 1.0);  
	gl_Position = vec4(gl_Position.x + SCALEX * cos((gl_Position.y+gl_Position.z)* (SPEEDX)) ,
		gl_Position.y ,  gl_Position.z + SCALEZ * cos((gl_Position.y+gl_Position.x)*(SPEEDZ)),1.0f);
}    
void main1()                                                                         
{                                                                                   
    gl_Position = vec4(Position, 1.0);  
	gl_Position = vec4(gl_Position.x + SCALEX * random( vec2(gl_Position.y * rnd2,gl_Position.z * rnd1)* (SPEEDX)),
		gl_Position.y ,  gl_Position.z + SCALEZ * random(  vec2(gl_Position.z * rnd1,gl_Position.y * rnd3) * (SPEEDZ)),1.0f);
}

void main0(){
    gl_Position = vec4(Position, 1.0);

	gl_Position = vec4(gl_Position.x + SCALEX * cos((gl_Position.y+gl_Position.z)* (SPEEDX)) ,
		gl_Position.y ,  gl_Position.z + SCALEZ * cos((gl_Position.y+gl_Position.x)*(SPEEDZ)),1.0f);

	gl_Position = gl_Position + vec4(gl_Position.x + SCALEX * random( vec2(gl_Position.y * rnd2,gl_Position.z * rnd1)* (SPEEDX)),
		gl_Position.y ,  gl_Position.z + SCALEZ * random(  vec2(gl_Position.z * rnd1,gl_Position.y * rnd3) * (SPEEDZ)),1.0f);

	gl_Position = gl_Position/2.0f;
}
