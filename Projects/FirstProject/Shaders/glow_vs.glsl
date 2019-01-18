#version 330                                                                        
                                                                                    
in vec3 Position;    
in vec2 Texcoord;
in vec3 Normal;

uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec3 vertex;
out vec3 normal;
out vec2 texCoord;


void main()                                                                         
{              
	mat4 normalMatrix = transpose(inverse(ModelMatrix));
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position,1.0f);
	vertex = (ModelMatrix * vec4(Position,1.0f)).xyz;
    normal = (normalMatrix * vec4(Normal, 0.0)).xyz;
	normal = Normal;
	texCoord = Texcoord;
}                                                                                   
