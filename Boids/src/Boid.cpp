#include "../Boid.h"


    // %%%%%%%%%%%%%%%%%%%%%% getter functions
    glm::vec3 Boid::getPosition() { return position; }
    glm::vec3 Boid::getHeading() { return heading; }
    glm::vec3 Boid::getNormal() { return normal; }
    void Boid::move()
    {
        position += heading * velocity;
    }
