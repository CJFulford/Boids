#pragma once

#include "Tools.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <vector>


#define PI					3.14159265359f
#define gravity				9.81f

#define WINDOW_WIDTH		700
#define WINDOW_HEIGHT		500


#define identity		mat4(1.f)
#define defaultZoom		2.f
#define defaultCamUp	vec3(0.f, 1.f, 0.f)
#define defaultCamLoc	vec3(0.f, .5f, 2.f)
#define defaultCamCent	vec3(0.f, 0.f, 0.f)


void generateShaders();

void passBasicUniforms(GLuint program);
void printOpenGLVersion(GLenum majorVer, GLenum minorVer, GLenum langVer);

void errorCallback(int error, const char* description);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void cursorPositionCallback(GLFWwindow* window, double x, double y);
void windowSizeCallback(GLFWwindow* window, int width, int height);