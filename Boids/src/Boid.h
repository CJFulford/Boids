#pragma once

#include <glm\glm.hpp>

class Boid
{
private:
    // variables
    glm::vec3 position;
    glm::vec3 heading;
    glm::vec3 normal;
    glm::vec3 acceleration;
    float FOV;

public:
    // %%%%%%%%%%%%%%%%%%% constructors
    Boid()
    {
        position = glm::vec3(0.f);
        heading = glm::vec3(1.f, 0.f, 0.f);
        normal = glm::vec3(0.f, 1.f, 0.f);
        acceleration = glm::vec3(0.f);
        FOV = 270.f;
    }
    Boid(glm::vec3 pos, glm::vec3 head, float fov)
    {
        position = pos;
        heading = glm::normalize(head);
        normal = glm::vec3(0.f, 1.f, 0.f);
        acceleration = glm::vec3(0.f);
        FOV = fov;
    }

    // %%%%%%%%%%%%%%%%%%%%%% getter functions
    glm::vec3 getPosition() { return position; }
    glm::vec3 getHeading() { return heading; }
    glm::vec3 getNormal() { return normal; }
};
