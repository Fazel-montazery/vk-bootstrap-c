#include "window.h"

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
		return SUCCESS;
	}

#ifndef NDEBUG
	fprintf(stderr, "[Error] creating glfw window.\n");
#endif
	return ERROR_GLFW_WINDOW_CREATION;
}
