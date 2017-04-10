#include "Header.h"

#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

using namespace glm;

#define identity	    mat4(1.f)

#define WINDOW_WIDTH	1200
#define WINDOW_HEIGHT	900
#define WINDOW_NAME     "Boid Simulation"

#define defaultZoom		max(3.f, (NUM_OF_BOIDS / 1000.f))
#define defaultCamUp	vec3(0.f, 1.f, 0.f)
#define defaultCamLoc	vec3(.2f, .5f, 2.f)
#define defaultCamCent	vec3(0.f, 0.f, 0.f)

#define FOV		        45.f
#define zNear	        .1f
#define zFar	        (defaultZoom * 10.f)


double  
    mouse_old_x, 
    mouse_old_y;
float   
    rotate_x = 0.0,
	rotate_y = 0.0,
	zoom = defaultZoom,
	aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
vec3	
    camUp	= defaultCamUp,
	camLoc	= defaultCamLoc,
	camCent	= defaultCamCent;
bool 
    bubbleFloor = true;


void passBasicUniforms(GLuint program)
{
    #define X_AXIS glm::vec3(1.f, 0.f, 0.f)
    #define Y_AXIS glm::vec3(0.f, 1.f, 0.f)
    #define PI 3.14159265359f
    #define identity glm::mat4(1.f)
	mat4
        projection = perspective(FOV, aspectRatio, zNear, zFar),

        modelview = lookAt(camLoc * zoom, camCent, camUp),
	    rotationX  = rotate(identity , rotate_x  * PI / 180.0f, X_AXIS);
		modelview *= rotate(rotationX, rotate_y  * PI / 180.0f, Y_AXIS);

	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, value_ptr(projection));
}

bool checkBubbleFloor() { return bubbleFloor; }

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
		case (GLFW_KEY_N):
			std::cout << "Recompiling Shaders... ";
			generateShaders();
			std::cout << "Done" << std::endl;
			break;
        case (GLFW_KEY_R):
            bubbleFloor = !bubbleFloor;
            break;
		default:
			break;
		}
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset < 0)
		zoom += 0.1f;
	else if (yoffset > 0)
		zoom = max(zoom - 0.1f, 0.005f);
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
