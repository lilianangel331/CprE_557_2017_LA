#version 410 core                                                 
                                                                  
uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                          
uniform mat4 modelMatrix;                                         
in vec3 in_Position;                                                                                                                
in vec3 in_Color;
in vec2 in_Tex_Coord;                                                 
out vec3 pass_Color;
out vec2 pass_Tex_Coord;                                              
                                                                  
void main(void)                                                   
{                                                                 
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
	pass_Tex_Coord = in_Tex_Coord;	
    pass_Color = in_Color; 
	
}     