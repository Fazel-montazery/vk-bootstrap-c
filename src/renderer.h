#pragma once

#include "config.h"

#include "queue.h"

#define MAX_FRAMES_IN_FLIGHT 2

struct Renderer {
	// Windowing
	GLFWwindow* window;
	uint32_t winWidth;
	uint32_t winHeight;

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
	VkFramebuffer* vkSwapChainFramebuffersVec;

	VkRenderPass vkRenderPass;
	VkPipelineLayout vkPipelineLayout;
	VkPipeline vkGraphicsPipeline;

	VkCommandPool vkCommandPool;
	VkCommandBuffer vkCommandBuffers[MAX_FRAMES_IN_FLIGHT];

	VkSemaphore vkImageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
	VkSemaphore vkRenderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
	VkFence vkInFlightFences[MAX_FRAMES_IN_FLIGHT];

	VkBuffer vkVertexBuffer;

	bool framebufferResized;

	// Useful data
	struct QueueFamilyIndices queueIndices;
};

State initRenderer(struct Renderer* renderer);
void runRenderer(struct Renderer* renderer);
void destroyRenderer(struct Renderer* renderer);
