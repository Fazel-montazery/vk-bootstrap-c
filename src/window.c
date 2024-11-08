#include "window.h"

State initWindow(int winWidth, int winHeight, const char* winTitle, struct Renderer* renderer)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if((renderer->window = glfwCreateWindow(winWidth, winHeight, winTitle, NULL, NULL))) {
		renderer->winWidth = winWidth;
		renderer->winHeight = winHeight;
		return SUCCESS;
	}

	return ERROR_GLFW_WINDOW_CREATION;
}
