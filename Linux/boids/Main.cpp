#include "Header.h"
#include "ShaderBuilder.h"
#include "Tools.h"
#include "../Boid.h"

#include <glm/gtx/rotate_vector.hpp>

#include <vector>
#include <ctime>
#include <omp.h>

#define FLOAT_ERROR 1e-5f
#define PI 3.14159265359f

#define radTOdeg (180.f / PI)
#define degTOrad (PI / 180.f)

#define MAIN_FLOOR_SIZE 10.f
#define NUM_OBSTACLE_POINTS_SCALAR 20000.f
#define INITIAL_POSITION_RANGE (NUM_OF_BOIDS / 10000.f)


#define MIN_ELEVATION .5f
#define Y_AXIS glm::vec3(0.f, 1.f, 0.f)

const GLfloat clearColor[] = { 0.f, 0.f, 0.f };
GLuint	
    bufferVertexArray = 0,
    bufferProgram = 0,
    floorBuffer = 0,
    
    obstacleVertexArray = 0,
    obstacleProgram = 0,
    obstacleBuffer = 0,
    
    boidVertexArray = 0,
    boidProgram = 0,
    positionBuffer = 0,
    headingBuffer = 0;
    

// %%%%%%%%%%%%%%%%%%%%%%%%% shader generation
void generateShaders()
{
    bufferProgram = generateProgram("shaders/buffer.vert", "shaders/buffer.frag");
    obstacleProgram = generateProgram("shaders/obstacle.vert", "shaders/obstacle.frag");
    boidProgram = generateProgram("shaders/boid.vert", "shaders/boid.geom", "shaders/boid.frag");
}

// %%%%%%%%%%%%%%%%%%%%%%%%% bubble representation as floor
void generateBufferFloorBuffer()
{
    glGenVertexArrays(1, &bufferVertexArray);
    glBindVertexArray(bufferVertexArray);

    GLfloat bufferVertices[] = {
        -BOID_AWARENESS_RADIUS, FLOOR_HEIGHT,    -BOID_AWARENESS_RADIUS,
        -BOID_AWARENESS_RADIUS, FLOOR_HEIGHT,     BOID_AWARENESS_RADIUS,
        BOID_AWARENESS_RADIUS,  FLOOR_HEIGHT,    -BOID_AWARENESS_RADIUS,
        BOID_AWARENESS_RADIUS,  FLOOR_HEIGHT,    BOID_AWARENESS_RADIUS,

        -BOID_BUFFER_RADIUS, FLOOR_HEIGHT + 0.001f,    -BOID_BUFFER_RADIUS,
        -BOID_BUFFER_RADIUS, FLOOR_HEIGHT + 0.001f,     BOID_BUFFER_RADIUS,
        BOID_BUFFER_RADIUS,  FLOOR_HEIGHT + 0.001f,    -BOID_BUFFER_RADIUS,
        BOID_BUFFER_RADIUS,  FLOOR_HEIGHT + 0.001f,    BOID_BUFFER_RADIUS,

        -MAIN_FLOOR_SIZE, FLOOR_HEIGHT - 0.001f,    -MAIN_FLOOR_SIZE,
        -MAIN_FLOOR_SIZE, FLOOR_HEIGHT - 0.001f,     MAIN_FLOOR_SIZE,
        MAIN_FLOOR_SIZE,  FLOOR_HEIGHT - 0.001f,    -MAIN_FLOOR_SIZE,
        MAIN_FLOOR_SIZE,  FLOOR_HEIGHT - 0.001f,    MAIN_FLOOR_SIZE
    };

    glGenBuffers(1, &floorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, floorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bufferVertices), bufferVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
void refreshBufferFloorBuffer()
{	
	GLfloat bufferVertices[] = {
        -BOID_AWARENESS_RADIUS,	FLOOR_HEIGHT,    		-BOID_AWARENESS_RADIUS,
        -BOID_AWARENESS_RADIUS, FLOOR_HEIGHT,   		BOID_AWARENESS_RADIUS,
        BOID_AWARENESS_RADIUS,  FLOOR_HEIGHT,    		-BOID_AWARENESS_RADIUS,
        BOID_AWARENESS_RADIUS,  FLOOR_HEIGHT,    		BOID_AWARENESS_RADIUS,

        -BOID_BUFFER_RADIUS, 	FLOOR_HEIGHT + 0.001f, 	-BOID_BUFFER_RADIUS,
        -BOID_BUFFER_RADIUS, 	FLOOR_HEIGHT + 0.001f,  BOID_BUFFER_RADIUS,
        BOID_BUFFER_RADIUS,  	FLOOR_HEIGHT + 0.001f,  -BOID_BUFFER_RADIUS,
        BOID_BUFFER_RADIUS,  	FLOOR_HEIGHT + 0.001f,  BOID_BUFFER_RADIUS,

        -MAIN_FLOOR_SIZE, 		FLOOR_HEIGHT - 0.001f,  -MAIN_FLOOR_SIZE,
        -MAIN_FLOOR_SIZE, 		FLOOR_HEIGHT - 0.001f,  MAIN_FLOOR_SIZE,
        MAIN_FLOOR_SIZE,  		FLOOR_HEIGHT - 0.001f,  -MAIN_FLOOR_SIZE,
        MAIN_FLOOR_SIZE,  		FLOOR_HEIGHT - 0.001f,  MAIN_FLOOR_SIZE
    };

    glBindVertexArray(bufferVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, floorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bufferVertices), bufferVertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
void renderBufferFloor(std::vector<Boid*> *boids)
{
    glBindVertexArray(bufferVertexArray);
    glUseProgram(bufferProgram);

    passBasicUniforms(bufferProgram, boids);
    glUniform1i(glGetUniformLocation(bufferProgram, "pass"), 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUniform1i(glGetUniformLocation(bufferProgram, "pass"), 1);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

    glUniform1i(glGetUniformLocation(bufferProgram, "pass"), 2);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

    glBindVertexArray(0);
}

// %%%%%%%%%%%%%%%%%%%%%%%%% obstacle
void generateObstacleGeometry(std::vector<glm::vec3> *vector)
{
	for (float i = 0.f; i < 2; i += 2.f / (NUM_OBSTACLE_POINTS_SCALAR * obstacleRadius))
	{
		vector->push_back(obstacleLocation + glm::rotateY(glm::vec3(obstacleRadius, FLOOR_HEIGHT, 0.f), i * PI));
		vector->push_back(obstacleLocation + glm::rotateY(glm::vec3(obstacleRadius, FLOOR_HEIGHT + 1000, 0.f), i * PI));
	}
}
int generateObstacleBuffer()
{
	std::vector<glm::vec3> obstacleVertices;

	generateObstacleGeometry(&obstacleVertices);
	
    glGenVertexArrays(1, &obstacleVertexArray);
    glBindVertexArray(obstacleVertexArray);

    glGenBuffers(1, &obstacleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, obstacleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(obstacleVertices[0]) * obstacleVertices.size(), &obstacleVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    
    return obstacleVertices.size();
}
int refreshObstacleBuffer()
{
	std::vector<glm::vec3> obstacleVertices;
	generateObstacleGeometry(&obstacleVertices);

    glBindVertexArray(obstacleVertexArray);
    
    glBindBuffer(GL_ARRAY_BUFFER, obstacleBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(obstacleVertices[0]) * obstacleVertices.size(), &obstacleVertices[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    
    return obstacleVertices.size();
}
void renderObstacle(int numPoints, std::vector<Boid*> *boids)
{
    glBindVertexArray(obstacleVertexArray);
    glUseProgram(obstacleProgram);
    
    passBasicUniforms(obstacleProgram, boids);
    
    glDrawArrays(GL_LINES, 0, numPoints);

    glBindVertexArray(0);
}

// %%%%%%%%%%%%%%%%%%%%%%%%% boids
void generateBoids(std::vector<Boid*> *boids)
{
    float
        xStartRange = -INITIAL_POSITION_RANGE,
        xEndRange = INITIAL_POSITION_RANGE,
        yStartRange = FLOOR_HEIGHT + MIN_ELEVATION,      // cant have boids start beneth the floor
        yEndRange = INITIAL_POSITION_RANGE,
        zStartRange = -INITIAL_POSITION_RANGE,
        zEndRange = INITIAL_POSITION_RANGE;

    #pragma omp for
    for (unsigned int i = 0; i < NUM_OF_BOIDS; i++)
    {
        boids->push_back(
            new Boid(
                generateRandomVector(xStartRange, xEndRange, yStartRange, yEndRange, zStartRange, zEndRange),
                generateRandomVector(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f),
                BOID_FOV
            )
        );
    }
}
void generateBoidBuffer(std::vector<Boid*> *boids)
{
    std::vector<glm::vec3> positions, headings;

    for (Boid *boid : *boids)
    {
        positions.push_back(boid->getPosition());
        headings.push_back(boid->getHeading());
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

    glBindVertexArray(0);
}
void refreshBoidBuffer(std::vector<Boid*> *boids)
{
    std::vector<glm::vec3> positions, headings;

    for (Boid *boid : *boids)
    {
        positions.push_back(boid->getPosition());
        headings.push_back(boid->getHeading());
    }
    
    glBindVertexArray(boidVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions[0]) * positions.size(), &positions[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, headingBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(headings[0]) * headings.size(), &headings[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
void renderBoids(std::vector<Boid*> *boids)
{
    glBindVertexArray(boidVertexArray);
    glUseProgram(boidProgram);

    passBasicUniforms(boidProgram, boids);
    glUniform1f(glGetUniformLocation(boidProgram, "scale"), BOID_SIZE_SCALE);

    glDrawArrays(GL_POINTS, 0, NUM_OF_BOIDS);

    glBindVertexArray(0);
}

// %%%%%%%%%%%%%%%%%%%%%%%%% simulation
void simulation(std::vector<Boid*> *boids, unsigned int index)
{
    float size = 0;
    glm::vec3 velocity(0.f), heading(0.f);
    Boid *boid = (*boids)[index], *otherBoid;

    for (unsigned int i = 0; i < boids->size(); i++)
    {
        if (i == index) continue;

        otherBoid = (*boids)[i];
        float distance = glm::distance(boid->getPosition(), otherBoid->getPosition());
        if (distance < BOID_AWARENESS_RADIUS)
        {
            glm::vec3 differenceVector = normalize(otherBoid->getPosition() - boid->getPosition());
            
            if (acos(dot(differenceVector, boid->getHeading())) * radTOdeg > boid->getFOV()) continue;

            size++;
            if (distance > BOID_BUFFER_RADIUS)
                velocity += otherBoid->getHeading() - boid->getPosition();
            else if (distance < BOID_BUFFER_RADIUS)
                velocity -= BOID_AWARENESS_RADIUS / glm::max(FLOAT_ERROR, distance) * (otherBoid->getHeading() - boid->getPosition());
        }
    }

    // keep the boids from going through the floor
    if (boid->getPosition().y < FLOOR_HEIGHT + MIN_ELEVATION)
    {
		float height = (boid->getPosition().y - (FLOOR_HEIGHT + MIN_ELEVATION)) / 
						((FLOOR_HEIGHT == -MIN_ELEVATION) ? 1.f : FLOOR_HEIGHT + MIN_ELEVATION);
        velocity += ((size == 0) ? 1.f : 1.f - size) * ((height == 0.f) ? FLOAT_ERROR : height) * Y_AXIS;
	}
	
	// have the boids react to the obstacle
	float distance = glm::distance(
									glm::vec2(boid->getPosition().x, boid->getPosition().z), 
									glm::vec2(obstacleLocation.x, obstacleLocation.z));
	if (distance - obstacleRadius < BOID_BUFFER_RADIUS)
    {
		if (index % 2 == 0)
			velocity += obstacleRepulsionScalar * size * cross(glm::vec3(obstacleLocation.x, boid->getPosition().y, obstacleLocation.z)-boid->getPosition(), Y_AXIS);
		else
			velocity += obstacleRepulsionScalar * size * cross(Y_AXIS, glm::vec3(obstacleLocation.x, boid->getPosition().y, obstacleLocation.z) - boid->getPosition());
	}


	if (size != 0)
		boid->updateVelocity(velocity / size);
	else // if a boid is not in range of any other boid, turn around and head back towards the center
		boid->updateVelocity(glm::normalize(glm::vec3(0.f, FLOOR_HEIGHT + MIN_ELEVATION, 0.f) - boid->getPosition()));
}

// %%%%%%%%%%%%%%%%%%%%%%%%% Program
int main()
{
    // initial setup
    GLFWwindow* window = generateWindow();

    generateShaders();
    generateBufferFloorBuffer();
    int obstaclePoints = generateObstacleBuffer();

    std::vector<Boid*> boids;
    generateBoids(&boids);
    generateBoidBuffer(&boids);

    // variable initialization for framerate counter
    time_t startTime = 0;
    int frames = 0;

    // rendering
	while (!glfwWindowShouldClose(window))
	{
		if (resetSimulation)
		{
			boids.clear();
			generateBoids(&boids);
			refreshBufferFloorBuffer();
			obstaclePoints = refreshObstacleBuffer();
			resetSimulation = false;
		}

		// initialize frame counter
		startTime = time(NULL);

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, clearColor);

		renderBufferFloor(&boids);
		renderObstacle(obstaclePoints, &boids);
		renderBoids(&boids);
		
		glDisable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (runSimulation)
		{
			#pragma omp parallel for
			for (int i = 0; i < boids.size(); i++)
				simulation(&boids, i);
			#pragma omp barrier // wait for simulation threads to finish

			for (Boid *boid : boids)
				boid->move();

			refreshBoidBuffer(&boids);
		}

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
