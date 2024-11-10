#include "renderer.h"

#include "window.h"
#include "instance.h"
#include "surface.h"
#include "device.h"

#ifndef NDEBUG
#include "debug.h"
#endif

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

#ifndef NDEBUG
	// SttingUp the debug messenger
	state = initDebugMessenger(&r);
	CHECK_STATE(state);
#endif

	// Create a window surface
	state = createSurface(&r);
	CHECK_STATE(state);

	// PickingUp a GPU
	state = pickPhysicalDevice(&r);
	CHECK_STATE(state);

	// Creating a logical device from the selected physical device
	state = createLogicalDevice(&r);
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

#ifndef NDEBUG
	destroyDebugMessenger(renderer);
#endif
	vkDestroyDevice(r.vkDevice, NULL);
	vkDestroySurfaceKHR(r.vkInstance, r.vkSurface, NULL);
	vkDestroyInstance(r.vkInstance, NULL);
	glfwDestroyWindow(r.window);
	glfwTerminate();
}
