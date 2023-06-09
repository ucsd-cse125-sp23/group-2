      #version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 7) in vec4 aColor;
layout (location = 8) in float aUseTexture;

out vec4 Color;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out float UseTexture;

uniform mat4 model;
uniform mat4 viewProj;

void main()
{
    Color = aColor;
    TexCoords = aTexCoords;
    UseTexture = aUseTexture;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = viewProj * model * vec4(aPos, 1.0);
}