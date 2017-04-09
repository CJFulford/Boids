#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>


#define NUM_OF_BOIDS 10000


// Main.cpp
void generateShaders();

//  Utilities.cpp
GLFWwindow* generateWindow();
void passBasicUniforms(GLuint program);