#include "window.h"

// For FPS counting
static double previousTime;
static int frameCount = 0;
static char fps_str[20];

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
            glfwSetWindowShouldClose(window, true);
        }
    }
}

State initWindow(int winWidth, int winHeight, const char* winTitle, struct Renderer* renderer)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if((renderer->window = glfwCreateWindow(winWidth, winHeight, winTitle, NULL, NULL))) {
		renderer->winWidth = winWidth;
		renderer->winHeight = winHeight;
		glfwSetKeyCallback(renderer->window, keyCallback);
		previousTime = glfwGetTime(); // For FPS counting
		return SUCCESS;
	}

	return ERROR_GLFW_WINDOW_CREATION;
}

void updateFpsInWindowTitle(GLFWwindow* win)
{
	double currentTime = glfwGetTime();
	frameCount++;

	if ( currentTime - previousTime >= 1.0 )
	{
		snprintf(fps_str, 19, "FPS: %d", frameCount);
		glfwSetWindowTitle(win, fps_str);

		frameCount = 0;
		previousTime = currentTime;
	}
}
