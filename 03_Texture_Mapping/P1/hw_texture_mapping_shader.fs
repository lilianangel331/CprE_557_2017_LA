#version 410 core                                                 
                                                                  
uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D tex3;																  
	
in vec2 pass_Tex_Coord;	
in vec3 pass_Color;                                             
out vec4 color;                                                   
void main(void)                                                   
{   
	vec4 tex_col = texture(tex, pass_Tex_Coord);
	vec4 tex2_col = texture(tex2, pass_Tex_Coord);
	vec4 tex3_col = texture(tex3, pass_Tex_Coord);

    //color = tex_col + tex2_col*.5  + tex3_col*.7 - 1.5;
     color = tex_col/tex2_col  * tex3_col;
} 