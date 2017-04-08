#include "Header.h"
#include "ShaderBuilder.h"
#include <vector>

#define NUM_OF_BOIDS 1

const GLfloat clearColor[] = { 0.f, 0.f, 0.f };
GLuint	
    floorVertexArray, 
    floorProgram,
    
    boidVertexArray,
    boidProgram;

// %%%%%%%%%%%%%%%%%%%%%%%%% shader generation
void generateShaders()
{
    floorProgram = generateProgram("shaders/floor.vert", "shaders/floor.frag");
    boidProgram = generateProgram("shaders/boid.vert", "shaders/boid.geom", "shaders/boid.frag");
}

// %%%%%%%%%%%%%%%%%%%%%%%%% floor
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

// %%%%%%%%%%%%%%%%%%%%%%%%% boids
void generateBoidBuffer()
{
#define FLOORSIZE 5.f
#define FLOORHEIGHT -1.f
    GLuint 
        positionBuffer = 0,
        headingBuffer = 0,
        normalBuffer = 0;

    glGenVertexArrays(1, &boidVertexArray);
    glBindVertexArray(boidVertexArray);

    std::vector<Boid*> boids;

    for (int i = 0; i < NUM_OF_BOIDS; i++)
    {
        boids.push_back(new Boid());
    }
    
    std::vector<glm::vec3> positions, headings, normals;
    for (Boid *boid : boids)
    {
        positions.push_back(boid->getPosition());
        positions.push_back(boid->getHeading());
        positions.push_back(boid->getNormal());
    }


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
    generateBoidBuffer();

    // rendering
	while (!glfwWindowShouldClose(window))
	{
        glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, clearColor);

        renderFloor();
        renderBoids();
		
        glDisable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Shutdow the program
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}