#pragma once

#include <glm\glm.hpp>

class Boid
{
private:
    // variables
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 heading;
    glm::vec3 normal;
    float FOV;

public:
#define zeroVector glm::vec3(0.f)
    // %%%%%%%%%%%%%%%%%%% constructors
    Boid()
    {
        position = zeroVector;
        velocity = zeroVector;
        acceleration = zeroVector;
        heading = glm::vec3(1.f, 0.f, 0.f);
        normal = glm::vec3(0.f, 1.f, 0.f);
        FOV = 270.f;
    }

    Boid(glm::vec3 pos, glm::vec3 head, float fov)
    {
        position = pos;
        velocity = zeroVector;
        acceleration = zeroVector;
        heading = glm::normalize(head);
        normal = glm::vec3(0.f, 1.f, 0.f);
        FOV = fov;
    }
    // %%%%%%%%%%%%%%%%%%%%%% functions
    glm::vec3 getPosition();
    glm::vec3 getHeading();
    glm::vec3 getNormal();

    void updateVelocity(glm::vec3 vel);
    void updateHeading(glm::vec3 head);


    void Boid::move();
};
