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
#define LIFE 1.0f
#define VELOCITY vec3(0.1f,2.0f,0.1f)
float random (vec2 st) {
    return fract(sin(dot(st.xy,
        vec2(12.9898,78.233)))*
        43758.5453123);
}

void main()
{
	float Life = Life0[0] - delta; 
	vec3 Velocity;
	if( Life > 0.0f ){	// particle is alive, thus update and emit it again
        vec3 Velocity = Velocity0[0] + vec3(0.0f,-9.82f,0.0f) * delta;
        vec3 Position = Position0[0] +  (Velocity * delta );
        Position1 = Position;
		Velocity1 = Velocity;
		Life1 = Life;
		EmitVertex();
		EndPrimitive();
	}
	//generate new particles
	vec2 st = vec2(Position0[0].xz);
	for (int i = 0 ; i < 10 ; i++) {
		float rnd1 = random(st);
		float rnd2 = random(st + vec2(rnd1,rnd1 / Position0[0].x));
		float rnd3 = random(st + vec2(rnd2,rnd1));
		Position1 = position + vec3( rnd1, 0.0f, ( rnd1 - rnd2 + rnd3 ) );
		Velocity1 = VELOCITY + vec3( rnd1, rnd2, rnd3 );
		Life1 = LIFE;
		EmitVertex();
		EndPrimitive();
		st = vec2(Position1.xz);
	}
}
