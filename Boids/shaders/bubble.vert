#version 430 core

layout (location = 0) in vec3 vertex;

uniform mat4 projection;
uniform mat4 modelview;
uniform int pass;

out vec3 colour;

void main (void)
{    
    gl_Position = projection * modelview * vec4(vertex, 1.f);
    if (pass == 1)
        colour = vec3(.5f, 0.f, 0.f);
    else
        colour = vec3(0.f, 0.f, 1.f);
}
