#pragma once

#include "config.h"

#include "queue.h"

#define CHECK_STATE(state) do {         \
        if (state != SUCCESS)           \
                return state;           \
} while(0)

struct Renderer {
	// Windowing
	GLFWwindow* window;
	int winWidth;
	int winHeight;

	// Vulkan handles
	VkInstance vkInstance;
#ifndef NDEBUG
	VkDebugUtilsMessengerEXT debugMessenger;
#endif
	VkSurfaceKHR vkSurface;
	VkPhysicalDevice vkPhysicalDevice;
	VkDevice vkDevice;
	VkQueue vkGraphicsQueue;
	VkQueue vkPresentQueue;

	VkSwapchainKHR vkSwapChain;
	VkImage* vkSwapChainImagesVec;
	VkImageView* vkSwapChainImageViewsVec;
	VkFormat vkSwapChainImageFormat;
	VkExtent2D vkSwapChainExtent;

	// Useful data
	struct QueueFamilyIndices queueIndices;
};

State initRenderer(struct Renderer* renderer);
void runRenderer(struct Renderer renderer);
void destroyRenderer(struct Renderer* renderer);
