#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "../Boid.h"


#define NUM_OF_PARAMETERS_IN_FILE 11


extern unsigned int NUM_OF_BOIDS;
extern float BOID_AWARENESS_RADIUS;
extern float BOID_BUFFER_RADIUS;
extern float BOID_FOV;
extern float FLOOR_HEIGHT;
extern float BOID_SIZE_SCALE;
extern bool resetSimulation;
extern bool runSimulation;

extern glm::vec3 obstacleLocation;
extern float obstacleRadius;
extern float obstacleRepulsionScalar;

// Main.cpp
void generateShaders();

//  Utilities.cpp
GLFWwindow* generateWindow();
void passBasicUniforms(GLuint program, std::vector<Boid*> *boids);
