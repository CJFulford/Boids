#version 430 core

layout (location = 0) in vec3 vertex;

uniform mat4 projection;
uniform mat4 modelview;

void main (void)
{    
    gl_Position = projection * modelview * vec4(vertex, 1.f);
}
