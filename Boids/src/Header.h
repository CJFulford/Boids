#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>
// Main.cpp
void generateShaders();

//  Utilities.cpp
GLFWwindow* generateWindow();
void passBasicUniforms(GLuint program);