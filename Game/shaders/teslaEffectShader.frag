#version 330 core
out vec4 FragColor;
    vec3 position;
    
struct Material {
    float shininess;
    bool useShininess;
    float opacity;
    bool useOpacity;
    float density;
    bool useDensity;
    float illum;
    bool useIllum;

    vec3 ambient;
    bool useAmbient;
    vec3 diffuse;
    bool useDiffuse;
    vec3 specular;
    bool useSpecular;
    vec3 emision;
};

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;


void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
}