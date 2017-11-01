#version 330 core
#define MAX_LIGHTS 10

// The vertex buffer input
in vec2 in_TexCoord;
in vec3 in_Position;
in vec3 in_Normal;

// Transformations for the projections
uniform mat4 projectionMatrixBox;
uniform mat4 viewMatrixBox;
uniform mat4 modelMatrixBox;
uniform mat4 inverseViewMatrix;

uniform int numLights;
 
// The light sources
uniform struct Light {
    vec4 light_position;
    float diffuse_intensity;
    float ambient_intensity;
    float specular_intensity;
    float attenuationCoefficient;
    float cone_angle;
    vec3 cone_direction;
} allLights[MAX_LIGHTS];


// The material parameters
uniform struct Material {
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    vec3 emissive;
    float shininess;
    float transparency;
} allMaterials[1];


// The output color
out vec4 pass_Color;
out vec2 pass_TexCoord;


out vec3 pass_Normal;
out vec4 pass_transformedNormal;
out vec4 pass_surfacePostion;


vec4 useLight(Light light, vec4 surfacePostion, vec4 normal_transformed, vec3 normal, Material material )
{
    // Diffuse color
    vec3 out_diffuse_color = material.diffuse;
       
    // Ambient color
    vec3 out_ambient_color = material.ambient * light.ambient_intensity;

    vec3 light_direction = normalize(light.light_position.xyz);
        
    float light_to_surface_angle = dot(light_direction, normal_transformed.xyz);
    
    vec2 new_texture = in_TexCoord * 2.0;
    
    vec3 linearColor =  out_ambient_color +  out_diffuse_color;
   
    return linearColor;
}

void main(void)
{
    // Caculate the normal vector and surface position in world coordinates
    vec3 normal = normalize(in_Normal);
    vec4 transformedNormal =  normalize(transpose(inverse(modelMatrixBox)) * vec4( normal, 1.0 ));
    vec4 surfacePostion = modelMatrixBox * vec4(in_Position, 1.0);
    
    // Calculate the color
    vec4 linearColor = vec4(0.0,0.0,0.0,0.0);

    // Pass the color
    pass_Color =  vec4(linearColor);
    
    // Passes the projected position to the fragment shader / rasterization process.
    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);
    
    // Passes the texture coordinates to the next pipeline processes.
    pass_TexCoord = in_TexCoord;
     
    pass_Normal = normal;
    pass_transformedNormal = transformedNormal;
    pass_surfacePostion = surfacePostion;  
}






















