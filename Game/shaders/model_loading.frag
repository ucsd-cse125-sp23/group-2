#version 330 core
out vec4 FragColor;

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
};

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform Material material;

void main()
{    

    if (material.useDiffuse) {
        FragColor = texture(texture_diffuse1, TexCoords);
    } else {
        FragColor = vec4(material.diffuse, 1.0);
    }

}