#include "renderer.h"

#include "window.h"
#include "instance.h"
#include "surface.h"
#include "device.h"
#include "swap_chain.h"
#include "graphics_pipeline.h"

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

	// Creating the swapchain
	state = createSwapChain(&r);
	CHECK_STATE(state);

	// Creating imageViews
	state = createImageViews(&r);
	CHECK_STATE(state);

	// Creating the renderpass
	state = createRenderPass(&r);
	CHECK_STATE(state);

	// Creating the initial graphics pipeline
	state = createGraphicsPipeline(&r);
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
	vkDestroyPipeline(r.vkDevice, r.vkGraphicsPipeline, NULL);
	vkDestroyPipelineLayout(r.vkDevice, r.vkPipelineLayout, NULL);
	vkDestroyRenderPass(r.vkDevice, r.vkRenderPass, NULL);
	for (int i = 0; i < vector_size(r.vkSwapChainImageViewsVec); i++) {
		vkDestroyImageView(r.vkDevice, r.vkSwapChainImageViewsVec[i], NULL);
	}
	vector_free(r.vkSwapChainImageViewsVec);
	vector_free(r.vkSwapChainImagesVec);
	vkDestroySwapchainKHR(r.vkDevice, r.vkSwapChain, NULL);
	vkDestroyDevice(r.vkDevice, NULL);
	vkDestroySurfaceKHR(r.vkInstance, r.vkSurface, NULL);
	vkDestroyInstance(r.vkInstance, NULL);
	glfwDestroyWindow(r.window);
	glfwTerminate();
}
