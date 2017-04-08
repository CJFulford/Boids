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