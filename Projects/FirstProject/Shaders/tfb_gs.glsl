#version 330
layout(points) in;
layout(points) out;
layout(max_vertices = 30) out;

in vec3 Position0[];
in vec3 Velocity0[];
in float Life0[];
in float Type0[];

out vec3 Position1;
out vec3 Velocity1;
out float Life1;
out float Type1;

uniform float delta;
uniform vec3 position;
uniform float rnd1;
uniform float rnd2;
uniform float rnd3;

#define LIFE 1.5f
#define TIMER 0.01f
#define VELOCITY vec3(0.0f,1.0f,0.0f)
#define SCALE 2.0f
#define PARTICLE_LAUNCHER 0.0f
#define PARTICLE_TYPE 1.0f

float random (vec2 st) {
    return fract(sin(dot(st.xy,
        vec2(12.9898,78.233)))*
        43758.5453123);
}

void main(){
	float Life = Life0[0] - delta;
	// if launcher and launcher timer is < 0 create new particles
	if(Type0[0] == PARTICLE_LAUNCHER){
		if(Life < 0.0f){
			// reset timer
			Position1 = position;
			Velocity1 = Velocity0[0];
			Life1 = TIMER;
			Type1 = PARTICLE_LAUNCHER;
			EmitVertex();
			EndPrimitive();
			vec2 st = vec2(rnd1,rnd3);
			for(int i = 0; i < 1000; i++){
				float strnd1 = random(st);
				float strnd2 = random(st + vec2(rnd2,strnd1));
				float strnd3 = random(st + vec2(strnd2,strnd1));
				Position1 = position + vec3(0.5f - strnd1,0.5f - strnd2,0.5f - strnd3)/2.0f;
				Velocity1 = VELOCITY + vec3(0.5f - strnd3,0.5f - strnd1,0.5f - strnd2);
				Life1 = LIFE + rnd2/2.0f ;
				Type1 = PARTICLE_TYPE;
				EmitVertex();
				EndPrimitive();
				st = vec2(Position1.xz);
			}
			
		}else{ 
			Position1 = position;
			Velocity1 = Velocity0[0];
			Life1 = Life;
			Type1 = PARTICLE_LAUNCHER;
			EmitVertex();
			EndPrimitive();
		}
	}else{// is not particle launcher
	// update Particle Position
		if(Life>0){
			Position1 = Position0[0] + (Velocity0[0] * delta) ;
			Velocity1 = Velocity0[0];
			Life1 = Life;
			Type1 = PARTICLE_TYPE;
			EmitVertex();
			EndPrimitive();
		}
	}


}
