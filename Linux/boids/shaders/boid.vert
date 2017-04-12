#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 head;


out vec3 position;
out vec3 heading;

void main (void)
{
    position = pos;
    heading = head;
    gl_Position = vec4(pos, 1.f);
}
