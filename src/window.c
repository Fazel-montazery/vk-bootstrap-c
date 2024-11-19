#include "window.h"

#ifndef NDEBUG
// For FPS counting
static double previousTime;
static int frameCount = 0;
static char fps_str[20];
#endif

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) 
{
	struct Renderer* r = glfwGetWindowUserPointer(window);
	r->framebufferResized = true;
	r->winWidth = width;
	r->winHeight = height;
}

State initWindow(struct Renderer* renderer)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, renderer->config.winResizable);
	glfwWindowHint(GLFW_FOCUSED, renderer->config.winFocused);

	uint32_t winWidth = renderer->config.winInitWidth;
	uint32_t winHeight = renderer->config.winInitHeight;

	if((renderer->window = glfwCreateWindow(winWidth, winHeight, renderer->config.winTitle, NULL, NULL))) {
		renderer->winWidth = winWidth;
		renderer->winHeight = winHeight;
		glfwSetWindowUserPointer(renderer->window, renderer);
		glfwSetKeyCallback(renderer->window, renderer->config.keyCallback);
		glfwSetFramebufferSizeCallback(renderer->window, framebufferResizeCallback);

#ifndef NDEBUG
		previousTime = glfwGetTime(); // For FPS counting
#endif

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

#ifndef NDEBUG
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
#endif
