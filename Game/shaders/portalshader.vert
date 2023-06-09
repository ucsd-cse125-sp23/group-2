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
uniform float time;
mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}
mat4 scaleMatrix(float x, float y, float z)
{
    return mat4(x,	0.0,	0.0,	 0.0,
		0.0,	y,	0.0, 	 0.0,
		0.0,	0.0,	z,	 0.0,
		0.0, 	0.0,	0.0,	 1.0);
}
void main()
{
    Color = aColor;
    TexCoords = aTexCoords;
    UseTexture = aUseTexture;
    mat4 m = model;
    m *= rotationMatrix(glm::vec3(1,0,0), time / 2.0f);
    m *= scaleMatrix(0.1 * sin(time * 6 + 1) + 1, 0.15 * sin(time*  6) + 1, 0.1 * sin(time*  6) + 1);
    FragPos = vec3(m * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(m))) * aNormal;
    gl_Position = viewProj * m * vec4(aPos, 1.0);
}