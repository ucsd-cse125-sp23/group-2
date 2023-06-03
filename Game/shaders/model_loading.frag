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
    vec3 emision;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_ambient1;
uniform sampler2D texture_shiny1;
uniform sampler2D texture_normal1;
uniform Material material;

const int numLights = 2;
uniform vec4 lightPos[numLights] = {
    vec4(0.0f, 30.0f, 0.0f, 0.0f),
    vec4(0.0f, 30.0f, 0.0f, 0.0f)
};
uniform vec4 lightColor[numLights] = {
    vec4(0.7f, 0.7f, 0.7f, 1.0f),
    vec4(0.3f, 0.3f, 0.2f, 1.0f)
};
//uniform vec3 lightPos = vec3(0.0f, 40.0f, 0.0f);
//uniform vec3 lightColor = vec3(0.9f, 0.4f, 0.1f);


void main()
{    
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specColor;
    float shine;
    ambientColor = vec4(material.ambient, 1.0f);
    if(material.useDiffuse){
        diffuseColor = texture(texture_diffuse1, TexCoords);
    }
    else{
        diffuseColor = vec4(material.diffuse, 1.0f);
    }
    specColor = vec4(material.specular, 1.0f);
    if(material.useShininess){
        shine = texture(texture_shiny1, TexCoords).r;
    }
    else{
        shine = material.shininess;
    }

    if(material.emision.x != 0 || material.emision.y != 0 || material.emision.z != 0){
        FragColor += vec4(material.emision, 1.0f);
    }

    for(int i = 0; i < numLights; i++){
    
        vec4 ambient = ambientColor * lightColor[i];

        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(vec3(lightPos[i]) - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec4 diffuse = lightColor[i] * (diff * diffuseColor);

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        vec3 halfway = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, reflectDir), 0.0), shine);
        vec4 specular = lightColor[i] * (spec * specColor);


        FragColor += 0.13f * ambient + diffuse + 0.1f* specular;
    }

}