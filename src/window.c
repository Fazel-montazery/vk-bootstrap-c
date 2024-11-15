#include "window.h"

// For FPS counting
static double previousTime;
static int frameCount = 0;
static char fps_str[20];

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
            glfwSetWindowShouldClose(window, true);
        }
    }
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) 
{
	struct Renderer* r = glfwGetWindowUserPointer(window);
	r->framebufferResized = true;
	r->winWidth = width;
	r->winHeight = height;
}

State initWindow(int winWidth, int winHeight, const char* winTitle, struct Renderer* renderer)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

	if((renderer->window = glfwCreateWindow(winWidth, winHeight, winTitle, NULL, NULL))) {
		renderer->winWidth = winWidth;
		renderer->winHeight = winHeight;
		glfwSetWindowUserPointer(renderer->window, renderer);
		glfwSetKeyCallback(renderer->window, keyCallback);
		glfwSetFramebufferSizeCallback(renderer->window, framebufferResizeCallback);
		previousTime = glfwGetTime(); // For FPS counting

		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* vidmode = glfwGetVideoMode(primaryMonitor);

		if (vidmode) {
			int monitorWidth = vidmode->width;
			int monitorHeight = vidmode->height;
			int posX = (monitorWidth - winWidth) / 2;
			int posY = (monitorHeight - winHeight) / 2;

			glfwSetWindowPos(renderer->window, posX, posY);
		}

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
