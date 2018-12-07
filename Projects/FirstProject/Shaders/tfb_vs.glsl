#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Velocity;
layout (location = 2) in float Life;

out vec3 Position0;
out vec3 Velocity0;
out float Life0;

void main()
{
	Position0 = Position;
	Velocity0 = Velocity;
	Life0 = Life;
}