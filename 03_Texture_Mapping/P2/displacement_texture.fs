#version 410 core                                                 
#define MAX_LIGHTS 10

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


uniform sampler2D texture_background; //this is the texture
uniform sampler2D texture_foreground; //this is the texture

in vec2 pass_TexCoord; //this is the texture coord
in vec4 pass_Color;
out vec4 color;

////uniform int texture_blend;
uniform float texture_delta;

in vec3 pass_Normal;
in vec4 pass_transformedNormal;
in vec4 pass_surfacePostion;

vec4 useLight(Light light, vec4 surfacePostion, vec4 normal_transformed, vec3 normal, Material material )
{
    normal_transformed = normalize(normal_transformed);
    normal = normalize(normal);
    
    // Diffuse color
    vec3 out_diffuse_color = material.diffuse;
    
    // Ambient color
    vec3 out_ambient_color = material.ambient;
    
    // Calculate the linear color
    vec3 linearColor =  out_ambient_color  +  out_diffuse_color;
    
    return linearColor;
}


void main(void)                                                   
{
    vec4 tex_displacement = vec4(0.0,0.0,0.0,0.0);
    
    tex_displacement = texture(texture_foreground, pass_TexCoord);
    
    vec2 displacement = texture_delta * tex_displacement.xy;

    // This function finds the color component for each texture coordinate. 
    vec4 tex_color =  texture(texture_background, pass_TexCoord + displacement);

    // Calculate the color
    vec4 linearColor = vec4(0.0,0.0,0.0,0.0);
    
    for (int i=0; i<numLights; i++) {
        vec4 new_light = useLight(allLights[i], pass_surfacePostion, pass_transformedNormal + tex_displacement, pass_Normal+tex_displacement.xyz, allMaterials[0] );
        linearColor = linearColor + new_light;
    }
    color = 0.1 * linearColor + tex_color;
}