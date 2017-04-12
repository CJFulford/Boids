#include "Header.h"
#include "../Boid.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// variables defined in the header that will be read here
unsigned int NUM_OF_BOIDS = 0;
float BOID_AWARENESS_RADIUS = 0;
float BOID_BUFFER_RADIUS = 0;
float BOID_FOV = 0;
float FLOOR_HEIGHT = 0;
float BOID_SIZE_SCALE = 0;
bool resetSimulation = false;
bool runSimulation = true;

glm::vec3 obstacleLocation(0.f);
float obstacleRadius = 0.f;
float obstacleRepulsionScalar = 0.f;

using namespace glm;

#define WINDOW_WIDTH	1200
#define WINDOW_HEIGHT	900
#define WINDOW_NAME     "Boid Simulation"

#define defaultZoom		max(1.f, (NUM_OF_BOIDS / 1000.f))
#define defaultCamUp	vec3(0.f, 1.f, 0.f)
#define defaultCamLoc	vec3(0.f, .5f, 2.f)
#define defaultCamCent	vec3(0.f, 0.f, 0.f)
#define MOVEMENT_SCALAR .2f

#define FOV		        45.f
#define zNear	        .1f
#define zFar	        (defaultZoom * 10.f)


double  
    mouse_old_x, 
    mouse_old_y;
float   
    rotate_x = 0.0,
	rotate_y = 0.0,
	camZoom = defaultZoom,
	aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
vec3	
    camUp	= defaultCamUp,
	camLoc	= defaultCamLoc,
	camCent	= defaultCamCent;
bool firstPerson = false;


bool readINIFile()
{
    std::fstream inputFile;
    inputFile.open("BoidPrefs.ini", std::ios::in);

    if (inputFile.is_open())
    {
        std::vector<std::string> input;
        std::string in;
        while (getline(inputFile, in)) { input.push_back(in); }
        inputFile.close();
        if (input.size() != NUM_OF_PARAMETERS_IN_FILE)
        {
            std::cout << "Wrong number of parameters." << std::endl;
            return false;
        }
        
        // file succesfully read all parameters if it reaches this point

        try 
        {
            BOID_SIZE_SCALE       = stof(input[0].substr(input[0].find_first_of(' '), std::string::npos));
            NUM_OF_BOIDS          = stoi(input[1].substr(input[1].find_first_of(' '), std::string::npos));
            BOID_AWARENESS_RADIUS = stof(input[2].substr(input[2].find_first_of(' '), std::string::npos));
            BOID_BUFFER_RADIUS    = stof(input[3].substr(input[3].find_first_of(' '), std::string::npos));
            BOID_FOV              = stof(input[4].substr(input[4].find_first_of(' '), std::string::npos));
            FLOOR_HEIGHT          = stof(input[5].substr(input[5].find_first_of(' '), std::string::npos));
            
            obstacleLocation = glm::vec3(
								stof(input[6].substr(input[6].find_first_of(' '), std::string::npos)),
								stof(input[7].substr(input[7].find_first_of(' '), std::string::npos)),
								stof(input[8].substr(input[8].find_first_of(' '), std::string::npos)));
            obstacleRadius = stof(input[9].substr(input[9].find_first_of(' '), std::string::npos));
            obstacleRepulsionScalar = stof(input[10].substr(input[10].find_first_of(' '), std::string::npos));
        }
        catch (const char* msg)
        {
            std::cout << "incorrect formatting of .ini file" << std::endl;
            return false;
        }

        return true;
    }
    else return false;
}
bool readBoidSize()
{
	std::fstream inputFile;
    inputFile.open("BoidPrefs.ini", std::ios::in);

    if (inputFile.is_open())
    {
		std::string in;
		if (!getline(inputFile, in))
		{
			std::cout << "failed to read .ini file" << std::endl;
			inputFile.close();
			return false;
		}
        inputFile.close();

        try
        {
            BOID_SIZE_SCALE = stof(in.substr(in.find_first_of(' '), std::string::npos));
		}
        catch (const char* msg)
        {
            std::cout << "incorrect formatting of Boid Size in .ini file" << std::endl;
            return false;
        }

        return true;
    }
    else return false;
}
void passBasicUniforms(GLuint program, std::vector<Boid*> *boids)
{
    #define X_AXIS glm::vec3(1.f, 0.f, 0.f)
    #define Y_AXIS glm::vec3(0.f, 1.f, 0.f)
    #define PI 3.14159265359f
    #define identity glm::mat4(1.f)


    mat4 modelview;
    if (firstPerson)
	{
		Boid *boid = (*boids)[0];
        vec3 cam, up, center;

		up = normalize(boid->getHeading() + Y_AXIS);
		float height = (.01f * BOID_SIZE_SCALE / 2.f) * BOID_SIZE_SCALE;
		cam = boid->getPosition() + height * normalize(boid->getPosition() + Y_AXIS);
		up = normalize(boid->getHeading()+ Y_AXIS);;
		center = boid->getPosition() + height * normalize(boid->getPosition() + Y_AXIS) + boid->getHeading();
        modelview = lookAt(cam, center, up);
	}
	else
	{
        modelview = lookAt(camLoc * camZoom, camCent, camUp);
        mat4 rotationX = rotate(identity, rotate_x  * PI / 180.0f, X_AXIS);
        modelview *= rotate(rotationX, rotate_y  * PI / 180.0f, Y_AXIS);
	}
    mat4 projection = perspective(FOV, aspectRatio, zNear, zFar);


	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, value_ptr(projection));
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% openGL info
void printOpenGLVersion(GLenum majorVer, GLenum minorVer, GLenum langVer)
{
    GLint major, minor;
    glGetIntegerv(majorVer, &major);
    glGetIntegerv(minorVer, &minor);
    printf("OpenGL on %s %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
    printf("GLSL version supported %s\n", glGetString(langVer));
    printf("GL version major, minor: %i.%i\n", major, minor);
}
void errorCallback(int error, const char* description)
{
    std::cout << "GLFW ERROR " << error << ": " << description << std::endl;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% controls
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			// shaders
		case (GLFW_KEY_R):
			if (!readINIFile())
				{
					std::cout << "Failed to read .ini file" << std::endl;
					exit(EXIT_FAILURE);
				}
            resetSimulation = true;
            runSimulation = true;
		case (GLFW_KEY_N):
			if (!readBoidSize())
			{
				std::cout << "Failed to read .ini file" << std::endl;
				exit(EXIT_FAILURE);
			}
			std::cout << "Recompiling Shaders... ";
			generateShaders();
			std::cout << "Done" << std::endl;
			break;
		case (GLFW_KEY_P):
			runSimulation = !runSimulation;
			break;
		case (GLFW_KEY_W):
			if (!firstPerson) break;
			camCent += MOVEMENT_SCALAR * Y_AXIS;
			camLoc += MOVEMENT_SCALAR * Y_AXIS;
			break;
		case (GLFW_KEY_S):
		if (!firstPerson) break;
			camCent -= MOVEMENT_SCALAR * Y_AXIS;
			camLoc -= MOVEMENT_SCALAR * Y_AXIS;
			break;
		case (GLFW_KEY_C):
			firstPerson = !firstPerson;
			break;
		default:
			break;
		}
	}
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset < 0)
		camZoom += 0.1f;
	else if (yoffset > 0)
		camZoom = max(camZoom - 0.1f, 0.005f);
}
void cursorPositionCallback(GLFWwindow* window, double x, double y)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
	{
		rotate_x += (float)((y - mouse_old_y));
		rotate_y += (float)((x - mouse_old_x));
	}
	mouse_old_x = x;
	mouse_old_y = y;
}
void windowSizeCallback(GLFWwindow* window, int width, int height)
{
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% window setup
GLFWwindow* generateWindow()
{
#define antiAliasing    4

    if (!readINIFile())
    {
        std::cout << "Failed to read .ini file" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_DOUBLEBUFFER, true);
    glfwWindowHint(GLFW_SAMPLES, antiAliasing);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);

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
