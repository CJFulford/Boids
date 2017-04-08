#include "Header.h"

#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#define movement 0.25f
#define FOV		 45.f
#define zNear	.01f
#define zFar	10.f

using namespace glm;

double  mouse_old_x,
		mouse_old_y;

float   rotate_x = 0.0,
		rotate_y = 0.0,
		zoom = defaultZoom,
		aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

vec3	camUp	= defaultCamUp,
		camLoc	= defaultCamLoc,
		camCent	= defaultCamCent;


void passBasicUniforms(GLuint program)
{
    #define X_AXIS glm::vec3(1.f, 0.f, 0.f)
    #define Y_AXIS glm::vec3(0.f, 1.f, 0.f)
	mat4	modelview = lookAt(camLoc * zoom, camCent, camUp),
			projection = perspective(FOV, aspectRatio, zNear, zFar);

	mat4	rotationX  = rotate(identity , rotate_x  * PI / 180.0f, X_AXIS);
			modelview *= rotate(rotationX, rotate_y  * PI / 180.0f, Y_AXIS);

	glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, value_ptr(projection));
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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
		default:
			break;
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset < 0)
		zoom += 0.1f;
	else if (yoffset > 0)
		zoom = max(zoom - 0.1f, 0.005f);
}

void mouse_motion(GLFWwindow* window, double x, double y)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
	{
		rotate_x += (float)((y - mouse_old_y));
		rotate_y += (float)((x - mouse_old_x));
	}
	mouse_old_x = x;
	mouse_old_y = y;
}

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

void window_size_callback(GLFWwindow* window, int width, int height)
{
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}