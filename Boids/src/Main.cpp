#include "Header.h"
#include "ShaderBuilder.h"

#include <iostream>
#include <string>

using namespace glm;

const GLfloat clearColor[] = { 0.f, 0.f, 0.f };

GLuint	floorVertexArray, floorProgram;

// rendering
void generateShaders()
{
    floorProgram = generateProgram("shaders/plane.vert", "shaders/plane.frag");
}

// %%%%%%%%%%%%%%%%%%%%%%%%% The floor
void generateFloorBuffer()
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
void renderFloor()
{
    glBindVertexArray(floorVertexArray);
    glUseProgram(floorProgram);

    passBasicUniforms(floorProgram);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
}


int main()
{
    GLFWwindow* window = generateWindow();

    generateShaders();


    generateFloorBuffer();

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