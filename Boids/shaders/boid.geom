#version 430 core

layout(points) in;
layout(triangle_Strip, max_vertices = 3) out;


uniform mat4 projection;
uniform mat4 modelview;

in vec3 position;
in vec3 heading;
in vec3 normal;

float boidWidth = .2f;
vec3 sideVec = boidWidth * normalize(cross(heading, up));

void main (void)
{
    gl_Position = projection * modelview * vec4(position + sideVec, 1.f);
	EmitVertex();


    gl_Position = projection * modelview * vec4(position + heading, 1.f);
	EmitVertex();


    gl_Position = projection * modelview * vec4(position - sideVec, 1.f);
	EmitVertex();
	
    
    EndPrimitive();
}
