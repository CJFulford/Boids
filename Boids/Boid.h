#pragma once

#include <glm\glm.hpp>

class Boid
{
private:
    // variables
    glm::vec3 position;
    glm::vec3 acceleration;
    glm::vec3 heading;
    glm::vec3 normal;
    float FOV;
    float velocity;

public:
#define defaultVelocity .1f
#define zeroVector glm::vec3(0.f)
    // %%%%%%%%%%%%%%%%%%% constructors
    Boid()
    {
        position = zeroVector;
        acceleration = zeroVector;
        heading = glm::vec3(1.f, 0.f, 0.f);
        normal = glm::vec3(0.f, 1.f, 0.f);
        FOV = 270.f;
        velocity = defaultVelocity;
    }

    Boid(glm::vec3 pos, glm::vec3 head, float fov)
    {
        position = pos;
        heading = glm::normalize(head);
        normal = glm::vec3(0.f, 1.f, 0.f);
        acceleration = glm::vec3(0.f);
        FOV = fov;
        velocity = defaultVelocity;
    }
    // %%%%%%%%%%%%%%%%%%%%%% getter functions
    glm::vec3 getPosition();
    glm::vec3 getHeading();
    glm::vec3 getNormal();
    void move();
};
