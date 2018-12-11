#version 330
layout(points) in;
layout(points) out;
layout(max_vertices = 30) out;

in vec3 Position0[];
in vec3 Velocity0[];
in float Life0[];

out vec3 Position1;
out vec3 Velocity1;
out float Life1;

uniform float delta;
uniform vec3 position;

#define LIFE 0.4f
#define VELOCITY vec3(0.0f,2.0f,0.0f)
#define SCALE 2.0f

float random (vec2 st) {
    return fract(sin(dot(st.xy,
        vec2(12.9898,78.233)))*
        43758.5453123);
}

void main(){
	float Life = Life0[0] - delta; 
	if(Life>0){
		Position1 = Position0[0] + (Velocity0[0] * delta) ;
		Velocity1 = Velocity0[0];
		Life1 = Life;
		EmitVertex();
		EndPrimitive();
	}else{
		vec2 st = vec2(Position0[0].xz);
		for(int i = 0; i < 10; i++){
			float rnd1 = random(st);
			float rnd2 = random(st + vec2(rnd1,rnd1 / Position0[0].x));
			if (rnd2 > 0.5f){
				rnd2 = -rnd2/2.0f;
			}
			float rnd3 = random(st + vec2(rnd2,rnd1));

			Position1 = position + vec3(cos(rnd2 * 10),cos(rnd1 * 10),cos(rnd3 * 10))/4.0f;

			if (rnd3 > 0.5f){
				rnd3 = -rnd3/2.0f;
			}
			if (rnd1 < 0.5f){
				rnd1 = rnd1 * 2.0f;
			}else{
				rnd1 = -rnd1;
			}
			Velocity1 = VELOCITY + vec3(rnd1,0.0f,rnd3);
			Life1 = LIFE +rnd2/2.0f ;
			EmitVertex();
			EndPrimitive();
			st = vec2(Position1.xz);
		}
	}

}
