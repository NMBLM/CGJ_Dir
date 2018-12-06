#version 330 core
in vec3 inPosition;

void main()
{
    //gl_Position = ProjectionMatrix * ViewMatrix  * vec4(inPosition.xyz, 1.0f);
	//gl_Position = ProjectionMatrix * ViewMatrix  * vec4(inPosition.xy,0.0f, 1.0f);
	//gl_Position = vec4(inPosition.xy,0.0f, 1.0f);
	gl_Position = vec4(inPosition.xyz, 1.0f);
	//gl_Position = ProjectionMatrix * ViewMatrix  * vec4(rotation * inPosition.xyz, 1.0f);
}