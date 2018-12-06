#version 330                                                                        
                                                                                                                                 
in vec3 Position;                                             
in vec3 Velocity;                                             
in float Life;                                                 
                                                                                                                                                      
out vec3 Position0;                                                                 
out vec3 Velocity0;                                                                 
out float Life0;                                                                     
                                                                                    
void main()                                                                         
{                                                                                                                                                   
    Position0 = Position;                                                           
    Velocity0 = Velocity;                                                           
    Life0 = Life;                                                                     
}