#version 430 core

out vec4 color;

in vec3 pos;
in vec3 norm;

vec3 sunColour = vec3(1.f, 1.f, 1.f);
vec3 sunLocation = vec3(10.f, 10.f, 2.f);

vec3 diffuseColour = vec3(0.7f, 0.7f, 0.75f);
vec3 specularColour = vec3(10.f, 10.f, 10.f);
vec3 ambientColour = vec3(0.2f, 0.2f, 0.2f);

float phongExp = 1000000000000.f;


void main (void)
{ 
    vec3 H = normalize((sunLocation + pos) / length(sunLocation + pos));

    float diffuse = max(0.f, max(   dot(normalize(sunLocation), norm),
                                    dot(normalize(sunLocation), -norm)
                                )
                        );
    float specular = pow(max(0.f, max(  dot(H, norm),
                                        dot(H, -norm)
                                    )
                            ), phongExp);

	color = vec4(
                ((diffuseColour * diffuse) + (specularColour * specular))
            , 0.f);
}
