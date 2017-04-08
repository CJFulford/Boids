#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

// math constants
#define PI					3.14159265359f
#define gravity				9.81f

// Main.cpp
void generateShaders();

//  Utilities.cpp
GLFWwindow* generateWindow();
void passBasicUniforms(GLuint program);

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
        position     = glm::vec3(0.f);
        heading      = glm::vec3(1.f, 0.f, 0.f);
        normal       = glm::vec3(0.f, 1.f, 0.f);
        acceleration = glm::vec3(0.f);
        FOV          = 270.f;
    }
    Boid(glm::vec3 pos, glm::vec3 head, glm::vec3 norm, glm::vec3 ass, float fov)
    {
        position     = pos;
        heading      = glm::normalize(head);
        normal       = glm::normalize(norm);
        acceleration = ass;
        FOV          = fov;
    }

    // %%%%%%%%%%%%%%%%%%%%%% getter functions
    glm::vec3 getPosition() { return position; }
    glm::vec3 getHeading() { return heading; }
    glm::vec3 getNormal() { return normal; }
};