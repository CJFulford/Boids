#include "../Boid.h"

#include <vector>


// %%%%%%%%%%%%%%%%%%%%%% getter functions
glm::vec3 Boid::getPosition() { return position; }
glm::vec3 Boid::getHeading() { return heading; }
float Boid::getFOV() { return FOV; }
void Boid::updateVelocity(glm::vec3 vel)
{
    velocity = vel;
}
void Boid::move()
{
#define TURNING_RATIO .95f
#define SPEED .005f
    heading = normalize((TURNING_RATIO * heading) + ((1.f - TURNING_RATIO)*velocity));
    position += heading * SPEED;
}
