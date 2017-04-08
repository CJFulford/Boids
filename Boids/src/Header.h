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


struct Mass 
{
	Mass() :	position(glm::vec3(0.f, 0.f, 0.f)), 
				velocity(glm::vec3(0.f, 0.f, 0.f)),
				force(glm::vec3(0.f, 0.f, 0.f)),
				mass(1.f), 
				fixed(false) { }
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
	float mass;
	bool fixed;
};

struct Spring
{
	Spring() :  restLength(.4f), 
				constant(50.f) { }
	unsigned int m1, m2;
	float restLength;
	float constant;
};

void generateShaders();

void passBasicUniforms(GLuint program);
void printOpenGLVersion(GLenum majorVer, GLenum minorVer, GLenum langVer);

void errorCallback(int error, const char* description);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void window_size_callback(GLFWwindow* window, int width, int height);
void mouse_motion(GLFWwindow* window, double x, double y);