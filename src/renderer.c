#include "renderer.h"

#include "window.h"
#include "instance.h"

State initRenderer(struct Renderer* renderer)
{
	State state = SUCCESS;
	struct Renderer r = {0};

	// Initialize glfw window
	state = initWindow(1280, 720, "My Shit", &r);
	CHECK_STATE(state);

	// Creating a vulkan instance
	state = createVulkanInstance("My Shit", &r);
	CHECK_STATE(state);

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

	vkDestroyInstance(r.vkInstance, NULL);
	glfwDestroyWindow(r.window);
	glfwTerminate();
}
