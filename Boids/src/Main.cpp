#include "Header.h"
#include "ShaderBuilder.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

using namespace glm;

#define antiAliasing		4

const GLfloat clearColor[] = { 0.f, 0.f, 0.f };

float	planeHeight = 0.f,
		planeSize = 2.f;

GLuint	planeProgram;

// rendering
void generateShaders()
{
	planeProgram = generateProgram("shaders/plane.vert",
									"shaders/plane.geom",
									"shaders/plane.frag");
}

void renderPlane(GLuint program)
{
    glUseProgram(program);

    passBasicUniforms(program);
	glUniform1f(glGetUniformLocation(program, "height"), planeHeight);
	glUniform1f(glGetUniformLocation(program, "planeSize"), planeSize);


    glDrawArrays(GL_POINTS, 0, 1);
}



int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(errorCallback);

	glfwWindowHint(GLFW_DOUBLEBUFFER, true);
	glfwWindowHint(GLFW_SAMPLES, antiAliasing);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Physics Sim", NULL, NULL);

	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_motion);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}
	printOpenGLVersion(GL_MAJOR_VERSION, GL_MINOR_VERSION, GL_SHADING_LANGUAGE_VERSION);

    generateShaders();

    glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window))
	{
        glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, clearColor);

		renderPlane(planeProgram);
		
        glDisable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Shutdow the program
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}