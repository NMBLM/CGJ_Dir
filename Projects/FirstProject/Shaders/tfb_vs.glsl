#version 330

in vec3 Position;
in vec3 Velocity;
in float Life;
in float Type;

out vec3 Position0;
out vec3 Velocity0;
out float Life0;
out float Type0;

void main()
{
	Position0 = Position;
	Velocity0 = Velocity;
	Life0 = Life;
	Type0 = Type;
}