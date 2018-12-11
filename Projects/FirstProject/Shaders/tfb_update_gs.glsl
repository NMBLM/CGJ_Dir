#version 330
layout(points) in;
layout(points) out;
layout(max_vertices = 30) out;

struct Particle{
	vec3 Position;
	vec3 Velocity;
	float Life;
};


in Particle particle0[];

out Particle particle1;

uniform float delta;
uniform vec3 position;
#define LIFE 1.0f
#define VELOCITY vec3(0.1f,2.0f,0.1f)
#define MaxParticles 10
float random (vec2 st) {
    return fract(sin(dot(st.xy,
        vec2(12.9898,78.233)))*
        43758.5453123);
}

void main()
{
	
	float Life = particle0[0].Life - delta; 
	vec3 Velocity;
	if( Life  > 0.0f ){	// particle is alive, thus update and emit it again
        vec3 Velocity = particle0[0].Velocity ;
        vec3 Position = particle0[0].Position +  (Velocity * delta );
        particle1.Position = Position;
		particle1.Velocity = VELOCITY;
		particle1.Life = Life;
		EmitVertex();
	}
	//generate new particles
	vec2 st = vec2(particle0[0].Position.xz);
	for (int i = 0 ; i < 10 ; i++) {
		float rnd1 = random(st);
		float rnd2 = random(st + vec2(rnd1,rnd1 / particle0[0].Position.x));
		float rnd3 = random(st + vec2(rnd2,rnd1));

		particle1.Position = position + vec3( rnd1, 0.0f, ( rnd1 - rnd2 + rnd3 ) );
		//Position1 = position + vec3( 0.0f,rnd1 + rnd2 + rnd3,0.0f) + vec3( rnd1, 0.0f, ( rnd1 - rnd2 + rnd3 ) );
		//Position1 = position ;
		particle1.Velocity = VELOCITY + vec3( rnd1, rnd2, rnd3 );
		particle1.Life = LIFE;
		EmitVertex();
		st = vec2(particle1.Position.xz);
	}
	EndPrimitive();
}
