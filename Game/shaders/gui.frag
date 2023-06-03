#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float transparency;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
    FragColor.a = transparency;

    if (FragColor.rgb = vec3(0.2f, 0.8f, 0.2f)) {
        FragColor.a = 0.0f;
    }
}