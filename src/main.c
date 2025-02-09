#include "renderer.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, char** argv)
{
	struct RendererConfig config = DEFAULT_RENDERER_CONFIG;
	config.keyCallback = keyCallback;

	struct Renderer myRenderer;
	if (initRenderer(&myRenderer, &config) != SUCCESS)
		return FAIL;

	runRenderer(&myRenderer);

	destroyRenderer(&myRenderer);
	return SUCCESS;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(window, true);
		}
	}
}
