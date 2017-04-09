#include "Header.h"
#include "ShaderBuilder.h"
#include "Tools.h"
#include "../Boid.h"

#include <vector>
#include <ctime>
#include <omp.h>


const float FLOORSIZE = NUM_OF_BOIDS / 1000.f;
#define FLOORHEIGHT -1.f
const float INITIAL_POSITION_RANGE = NUM_OF_BOIDS / 1000.f;


const GLfloat clearColor[] = { 0.f, 0.f, 0.f };
GLuint	
    floorVertexArray, 
    floorProgram,
    
    boidVertexArray,
    boidProgram;

GLuint
positionBuffer = 0,
headingBuffer = 0,
normalBuffer = 0;

// %%%%%%%%%%%%%%%%%%%%%%%%% shader generation
void generateShaders()
{
    floorProgram = generateProgram("shaders/floor.vert", "shaders/floor.frag");
    boidProgram = generateProgram("shaders/boid.vert", "shaders/boid.geom", "shaders/boid.frag");
}

// %%%%%%%%%%%%%%%%%%%%%%%%% floor
void generateFloorBuffer()
{
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

// %%%%%%%%%%%%%%%%%%%%%%%%% boids
void generateBoids(std::vector<Boid*> *boids)
{
    float
        xStartRange = -INITIAL_POSITION_RANGE,
        xEndRange = INITIAL_POSITION_RANGE,
        yStartRange = FLOORHEIGHT + .5f,      // cant have boids start beneth the floor
        yEndRange = INITIAL_POSITION_RANGE,
        zStartRange = -INITIAL_POSITION_RANGE,
        zEndRange = INITIAL_POSITION_RANGE,
        FOV = 270.f;

    #pragma omp for
    for (int i = 0; i < NUM_OF_BOIDS; i++)
    {
        boids->push_back(
            new Boid(
                generateRandomVector(xStartRange, xEndRange, yStartRange, yEndRange, zStartRange, zEndRange),
                generateRandomVector(-1.f, 1.f, 0.f, 0.f, -1.f, 1.f),
                FOV
            )
        );
    }
}
void generateBoidBuffer(std::vector<Boid*> *boids)
{
    std::vector<glm::vec3> positions, headings, normals;

    for (Boid *boid : *boids)
    {
        positions.push_back(boid->getPosition());
        headings.push_back(boid->getHeading());
        normals.push_back(boid->getNormal());
    }
    
    glGenVertexArrays(1, &boidVertexArray);
    glBindVertexArray(boidVertexArray);

    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &headingBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, headingBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(headings[0]) * headings.size(), &headings[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
void refreshBoidBuffer(std::vector<Boid*> *boids)
{
    std::vector<glm::vec3> positions, headings, normals;

    for (Boid *boid : *boids)
    {
        positions.push_back(boid->getPosition());
        headings.push_back(boid->getHeading());
        normals.push_back(boid->getNormal());
    }

    glGenVertexArrays(1, &boidVertexArray);
    glBindVertexArray(boidVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions[0]) * positions.size(), &positions[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, headingBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(headings[0]) * headings.size(), &headings[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(normals[0]) * normals.size(), &normals[0]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
void renderBoids()
{
    glBindVertexArray(boidVertexArray);
    glUseProgram(boidProgram);

    passBasicUniforms(boidProgram);

    glDrawArrays(GL_POINTS, 0, NUM_OF_BOIDS);

    glBindVertexArray(0);
}

// %%%%%%%%%%%%%%%%%%%%%%%%% Program
int main()
{
    // initial setup
    GLFWwindow* window = generateWindow();
    generateShaders();
    generateFloorBuffer();

    std::vector<Boid*> boids;
    generateBoids(&boids);
    generateBoidBuffer(&boids);

    // variable initialization for framerate counter
    time_t startTime = 0, endTime = 0;
    int frames = 0;

    // rendering
	while (!glfwWindowShouldClose(window))
	{
        // initialize frame counter
        startTime = time(NULL);

        glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, clearColor);

        renderFloor();
        renderBoids();
		
        glDisable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();

        #pragma omp for
        for (int i = 0; i < boids.size(); i++)
            boids[i]->move();

        refreshBoidBuffer(&boids);

        // update and print frames per second
        endTime = time(NULL);
        frames++;
        if (difftime(endTime, startTime) >= 1)
        {
            printf("\rFPS: %i", frames);
            frames = 0;
        }
	}

	// Shutdow the program
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}