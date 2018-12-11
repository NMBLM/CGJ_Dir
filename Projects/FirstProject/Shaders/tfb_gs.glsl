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
#define MaxParticles 50000
float random (vec2 st) {
    return fract(sin(dot(st.xy,
        vec2(12.9898,78.233)))*
        43758.5453123);
}

void main1()
{
	
	float Life = Life0[0] - delta; 
	vec3 Velocity;
	if( Life  > 0.0f ){	// particle is alive, thus update and emit it again
        vec3 Velocity = Velocity0[0] ;
        vec3 Position = Position0[0] +  (VELOCITY * delta );
        Position1 = Position;
		Velocity1 = VELOCITY;
		Life1 = LIFE;
		EmitVertex();
	}
	//generate new particles
	vec2 st = vec2(Position0[0].xz);
	for (int i = 0 ; i < 10 ; i++) {
		float rnd1 = random(st);
		float rnd2 = random(st + vec2(rnd1,rnd1 / Position0[0].x));
		float rnd3 = random(st + vec2(rnd2,rnd1));

		Position1 = position + vec3( rnd1, 0.0f, ( rnd1 - rnd2 + rnd3 ) );
		//Position1 = position + vec3( 0.0f,rnd1 + rnd2 + rnd3,0.0f) + vec3( rnd1, 0.0f, ( rnd1 - rnd2 + rnd3 ) );
		//Position1 = position ;
		Velocity1 = VELOCITY + vec3( rnd1, rnd2, rnd3 );
		Life1 = LIFE;
		EmitVertex();
		st = vec2(Position1.xz);
	}
	EndPrimitive();
	//		float rnd1 = random(st);
//		float rnd2 = random(st + vec2(rnd1,rnd1 / Position0[0].x));
//		float rnd3 = random(st + vec2(rnd2,rnd1));
//		Position1 = position + vec3( 0.5f - rnd1 , 0.0f, 0.5f - rnd2  );
//		Velocity1 = VELOCITY + vec3( rnd1, rnd2, rnd3 );
//		Life1 = LIFE;
//		EmitVertex();
}
uniform float rand;
void main2()
{


	//generate new particles
	vec2 st = vec2(rand, 1.0f + rand*rand);
	for (int i = 0 ; i < 2 ; i++) {
		float life = Life0[i] - delta; 
		if(life>0.0f){
			Position1 = Position0[i] + (Velocity0[i] * delta) ;
			Velocity1 = Velocity0[i];
			Life1 = life;
			EmitVertex();
			EndPrimitive();
		}else{
			float rnd1 = random(st);
			float rnd2 = random(st + Position0[0].xy);
			if(rnd2 > 0.5f){
				rnd2 = -rnd2 / 2.0f;
			}
			float rnd3 = random(st + vec2(rnd2,rnd1));
			Position1 = position +vec3(rnd1*0.5f,0.5f + rnd2,rnd3*0.5f);//+ vec3( 0.5f - rnd1 , 0.0f, 0.5f - rnd2  );
			Velocity1 = VELOCITY + vec3( rnd1, rnd2, rnd3 );
			Life1 = LIFE + rnd2;
			EmitVertex();
			EndPrimitive();
		}
		
		st = vec2(Position1.xz);

	}
}
void main3(){
	float life = Life0[0] - delta; 
	if(life>0){
		Position1 = Position0[0] + (Velocity0[0] * delta) ;
		Velocity1 = Velocity0[0];
		Life1 = life;
		EmitVertex();
		EndPrimitive();
	}else{
		vec2 st = vec2(Position0[0].xz);
		for(int i = 0; i < 10; i++){
			float rnd1 = random(st);
			float rnd2 = random(st + vec2(rnd1,rnd1 / Position0[0].x));
			float rnd3 = random(st + vec2(rnd2,rnd1));
			Position1 = position + vec3(0.5f - rnd1,0.0f,0.5f - rnd3);//+ vec3( 0.5f - rnd1 , 0.0f, 0.5f - rnd2  );
			Velocity1 = VELOCITY + vec3( rnd1,rnd2,rnd3  );
			Life1 = LIFE ;
			EmitVertex();
			EndPrimitive();
			st = vec2(Position1.xz);
		}
	}

	
}
void main()
{
	main3();
}