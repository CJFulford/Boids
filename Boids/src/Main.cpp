#include "Header.h"
#include "ShaderBuilder.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

using namespace glm;

#define antiAliasing		4

const GLfloat clearColor[] = { 0.f, 0.f, 0.f };

GLuint	floorVertexArray, floorProgram;

// rendering
void generateShaders()
{
    floorProgram = generateProgram("shaders/plane.vert", "shaders/plane.frag");
}

void renderFloor()
{
    glBindVertexArray(floorVertexArray);
    glUseProgram(floorProgram);

    passBasicUniforms(floorProgram);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
}

void generateGroundBuffer()
{
    #define FLOORSIZE 5.f
    #define FLOORHEIGHT -1.f
    GLuint vertexBuffer = 0;

    glGenVertexArrays(1, &floorVertexArray);
    glBindVertexArray(floorVertexArray);

    GLfloat floorVertices[] = {
        -FLOORSIZE, FLOORHEIGHT,    -FLOORSIZE,
        -FLOORSIZE, FLOORHEIGHT,     FLOORSIZE,
        FLOORSIZE,  FLOORHEIGHT,    -FLOORSIZE,
        FLOORSIZE,  FLOORHEIGHT,    FLOORSIZE
    };

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

GLFWwindow* generateWindow()
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
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    
    
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    printOpenGLVersion(GL_MAJOR_VERSION, GL_MINOR_VERSION, GL_SHADING_LANGUAGE_VERSION);
    
    glfwSwapInterval(1);

    return window;
}

int main()
{
    GLFWwindow* window = generateWindow();

    generateShaders();
    generateGroundBuffer();

	while (!glfwWindowShouldClose(window))
	{
        glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, clearColor);

        renderFloor();
		
        glDisable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Shutdow the program
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}