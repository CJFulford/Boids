#version 430 core

out vec4 color;

in vec3 norm;

vec3 sunLocation = vec3(10.f, 10.f, 2.f);

vec3 diffuseColour = vec3(0.5f, 1.f, 0.5f);
float ambientColour = .2f;

void main (void)
{ 
    float diffuse = max(0.f, max(   dot(normalize(sunLocation), norm),
                                    dot(normalize(sunLocation), -norm)
                                )
                        );

	color = vec4((ambientColour * diffuseColour) + (diffuseColour * diffuse), 0.f);
}
