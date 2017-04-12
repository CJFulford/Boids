#version 430 core

layout(points) in;
layout(triangle_Strip, max_vertices = 12) out;

vec3 Y_AXIS = vec3(0.f, 1.f, 0.f);

uniform mat4 projection;
uniform mat4 modelview;
uniform float scale;

in vec3 position[];
in vec3 heading[];

out vec3 norm;

float widthScale = .01f * scale;
float headingScale = .04f * scale;
float heightScale = (widthScale / 2.f) * scale;

void triangleOut(vec3 p1, vec3 p2, vec3 p3)
{
    norm = normalize(cross(p1 - p2, p3 - p2));

    gl_Position = projection * modelview * vec4(p1, 1.f);
	EmitVertex();
    gl_Position = projection * modelview * vec4(p2, 1.f);
	EmitVertex();
    gl_Position = projection * modelview * vec4(p3, 1.f);
	EmitVertex();
    EndPrimitive();
}

void main (void)
{
    vec3 normal = normalize(heading[0] + Y_AXIS);
    vec3 sideVec = widthScale * normalize(cross(heading[0], normal));
    vec3 frontVec = heading[0] * headingScale;
    vec3 topVec = normal * heightScale;

    
    // bottom
    vec3 p1 = position[0] + sideVec;
    vec3 p2 = position[0] + frontVec;
    vec3 p3 = position[0] - sideVec;
    triangleOut(p1, p2, p3);

    // side 1
    p1 = position[0] + sideVec;
    p2 = position[0] + frontVec;
    p3 = position[0] + topVec;
    triangleOut(p1, p2, p3);

    // side 2
    
    p1 = position[0] + topVec;
    p2 = position[0] + frontVec;
    p3 = position[0] - sideVec;
    triangleOut(p1, p2, p3);

    // back
    p1 = position[0] + sideVec;
    p2 = position[0] - sideVec;
    p3 = position[0] + topVec;
    triangleOut(p1, p2, p3);
    
}
