#include "renderer.h"

#include "window.h"

State initRenderer(struct Renderer* renderer)
{
	State state = SUCCESS;
	struct Renderer r = {0};

	state = initWindow(1280, 720, "My Shit", &r);
	if (state != SUCCESS)
		return state;

	*renderer = r;
	return state;
}

void runRenderer(struct Renderer renderer)
{
	while(!glfwWindowShouldClose(renderer.window)) {
		glfwPollEvents();
	}
}

void destroyRenderer(struct Renderer* renderer)
{
	struct Renderer r = *renderer;

	glfwDestroyWindow(r.window);
	glfwTerminate();
}
