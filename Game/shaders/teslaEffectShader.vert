#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;
uniform float time;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. 
out vec3 fragNormal;

mat4 scaleMatrix(float x, float y, float z)
{
    return mat4(x,	0.0,	0.0,	 0.0,
		0.0,	y,	0.0, 	 0.0,
		0.0,	0.0,	z,	 0.0,
		0.0, 	0.0,	0.0,	 1.0);
}


void main()
{

    mat4 m = model;
    float mult = 20;
    m *= scaleMatrix(mult * time, mult * time, mult * time);
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = viewProj * m * vec4(position, 1.0);

    // for shading
	fragNormal = vec3(m * vec4(normal, 0));

}