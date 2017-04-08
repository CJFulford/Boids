#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 head;
layout (location = 2) in vec3 norm;

out vec3 position;
out vec3 heading;
out vec3 normal;

void main (void)
{
    position = pos;
    heading = head;
    up = u;
    gl_Position = vec4(vertex, 1.f);
}
