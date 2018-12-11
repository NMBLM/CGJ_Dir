#version 330

struct Particle{
	vec3 Position;
	vec3 Velocity;
	float Life;
};

in Particle particle;
out vec3 Position0;
out vec3 Velocity0;
out float Life0;

void main()
{
	Position0 = particle.Position;
	Velocity0 = particle.Velocity;
	Life0 = particle.Life;
}