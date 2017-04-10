#include "Header.h"
#include "ShaderBuilder.h"
#include "Tools.h"
#include "../Boid.h"

#include <vector>
#include <ctime>
#include <omp.h>


const float FLOORSIZE = NUM_OF_BOIDS / 10000.f;
#define FLOORHEIGHT -1.f
const float INITIAL_POSITION_RANGE = NUM_OF_BOIDS / 10000.f;

#define NEIGHBOURHOOD_RADIUS .25f
#define BUBBLE_RADIUS .1f

const GLfloat clearColor[] = { 0.f, 0.f, 0.f };
GLuint	
    floorVertexArray, 
    floorProgram,

    bubbleVertexArray,
    bubbleProgram,
    
    boidVertexArray,
    boidProgram,

    positionBuffer = 0,
    headingBuffer = 0,
    normalBuffer = 0;

// %%%%%%%%%%%%%%%%%%%%%%%%% shader generation
void generateShaders()
{
    floorProgram = generateProgram("shaders/floor.vert", "shaders/floor.frag");
    bubbleProgram = generateProgram("shaders/bubble.vert", "shaders/bubble.frag");
    boidProgram = generateProgram("shaders/boid.vert", "shaders/boid.geom", "shaders/boid.frag");
}

// %%%%%%%%%%%%%%%%%%%%%%%%% bubble representation as floor
void generateBubbleFloorBuffer()
{
    GLuint vertexBuffer = 0;

    glGenVertexArrays(1, &bubbleVertexArray);
    glBindVertexArray(bubbleVertexArray);

    GLfloat bubbleVertices[] = {
        -NEIGHBOURHOOD_RADIUS, 0.f,    -NEIGHBOURHOOD_RADIUS,
        -NEIGHBOURHOOD_RADIUS, 0.f,     NEIGHBOURHOOD_RADIUS,
        NEIGHBOURHOOD_RADIUS,  0.f,    -NEIGHBOURHOOD_RADIUS,
        NEIGHBOURHOOD_RADIUS,  0.f,    NEIGHBOURHOOD_RADIUS,

        -BUBBLE_RADIUS, 0.001f,    -BUBBLE_RADIUS,
        -BUBBLE_RADIUS, 0.001f,     BUBBLE_RADIUS,
        BUBBLE_RADIUS,  0.001f,    -BUBBLE_RADIUS,
        BUBBLE_RADIUS,  0.001f,    BUBBLE_RADIUS
    };

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bubbleVertices), bubbleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
void renderBubbleFloor()
{
    glBindVertexArray(bubbleVertexArray);
    glUseProgram(bubbleProgram);

    passBasicUniforms(bubbleProgram);
    glUniform1i(glGetUniformLocation(bubbleProgram, "pass"), 1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUniform1i(glGetUniformLocation(bubbleProgram, "pass"), 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

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

// %%%%%%%%%%%%%%%%%%%%%%%%% simulation
void simulation(std::vector<Boid*> *boids, int index)
{
    float size = 0;
    glm::vec3 velocity(0.f), heading(0.f);
    Boid *boid = (*boids)[index], *otherBoid;

    for (int i = 0; i < boids->size(); i++)
    {
        if (i == index) continue;

        otherBoid = (*boids)[i];
        float distance = glm::distance(boid->getPosition(), otherBoid->getPosition());
        if (distance < NEIGHBOURHOOD_RADIUS)
        {
            glm::vec3 differenceVector = normalize(otherBoid->getPosition() - boid->getHeading());

            if (acos(dot(differenceVector, boid->getHeading())) > 135) continue;

            size++;
            if (distance > BUBBLE_RADIUS)
                velocity += otherBoid->getHeading() - boid->getPosition();
            else if (distance < BUBBLE_RADIUS)
                velocity -= ((distance == 0) ? (100000000.f) : 3.f / distance) * (otherBoid->getHeading() - boid->getPosition());
        }
    }
    boid->updateVelocity(velocity / size);

    boid->move();
}

// %%%%%%%%%%%%%%%%%%%%%%%%% Program
int main()
{
    // initial setup
    GLFWwindow* window = generateWindow();

    generateShaders();
    generateBubbleFloorBuffer();

    std::vector<Boid*> boids;
    generateBoids(&boids);
    generateBoidBuffer(&boids);

    // variable initialization for framerate counter
    time_t startTime = 0;
    int frames = 0;

    // rendering
	while (!glfwWindowShouldClose(window))
	{
        // initialize frame counter
        startTime = time(NULL);

        glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, clearColor);

        if(checkBubbleFloor()) renderBubbleFloor();
        renderBoids();
		
        glDisable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();

        #pragma omp for
        for (int i = 0; i < boids.size(); i++)
            simulation(&boids, i);

        refreshBoidBuffer(&boids);

        // update and print frames per second
        frames++;
        if (difftime(time(NULL), startTime) >= 1)
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