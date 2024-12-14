#pragma once

#include "config.h"

#include "queue.h"

#define MAX_FRAMES_IN_FLIGHT 2

#define MAX_STR_ARR_SIZE 60

#define DEFAULT_RENDERER_CONFIG \
    (struct RendererConfig) {  \
        .winTitle = "VulkanRenderer",   						\
        .winInitWidth = 1280,   							\
        .winInitHeight = 720,								\
        .winResizable = GLFW_FALSE,							\
        .winFocused = GLFW_TRUE,							\
	.keyCallback = NULL, 								\
	    										\
	.appName = "VulkanRenderer", 							\
	.appVersion = VK_MAKE_VERSION(1, 0, 0), 					\
	.engineName = "NoEngine", 							\
	.engineVersion = VK_MAKE_VERSION(1, 0, 0), 					\
	.apiVersion = VK_API_VERSION_1_0,	 					\
	.extraInstanceExtensions = NULL,	 					\
	.extraInstanceExtensionsCount = 0,						\
											\
	.preferredPhysicalDeviceType = PHYSICAL_DEVICE_TYPE_AUTO,			\
	.extraDeviceExtensions = NULL,							\
	.extraDeviceExtensionsCount = 0,						\
	    										\
	.preferredSwapchainSurfaceFormat = VK_FORMAT_B8G8R8_SRGB,			\
	.preferredSwapchainSurfaceColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,	\
	.preferredSwapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR			\
    }

struct RendererConfig {
	// GLFW window config
	char 			winTitle[MAX_STR_ARR_SIZE + 1];
	uint32_t 		winInitWidth;
	uint32_t		winInitHeight;
	int			winResizable; // GLFW_TRUE or GLFW_FALSE
	int			winFocused; // GLFW_TRUE or GLFW_FALSE
	GLFWkeyfun		keyCallback;

	// Vulkan Instance
	char			appName[MAX_STR_ARR_SIZE + 1];
	uint32_t		appVersion;
	char			engineName[MAX_STR_ARR_SIZE + 1];
	uint32_t		engineVersion;
	uint32_t		apiVersion;
	char**			extraInstanceExtensions; // Other than validation layers and glfw required extensions
	uint32_t		extraInstanceExtensionsCount;

	// Physical Device
	PhysicalDeviceType	preferredPhysicalDeviceType; // PhysicalDeviceType enum in state.h
	char**			extraDeviceExtensions;
	uint32_t		extraDeviceExtensionsCount;

	// Swap Chain
	VkFormat		preferredSwapchainSurfaceFormat;
	VkColorSpaceKHR		preferredSwapchainSurfaceColorSpace;
	VkPresentModeKHR	preferredSwapchainPresentMode;
};

struct Renderer {
	// Renderer config
	struct RendererConfig config;

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
	char** requiredDeviceExtensions;
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
	VkDeviceMemory vkVertexBufferMemory;

	bool framebufferResized;

	// Useful data
	struct QueueFamilyIndices queueIndices;
};

State initRenderer(struct Renderer* renderer, struct RendererConfig* config);
void runRenderer(struct Renderer* renderer);
void destroyRenderer(struct Renderer* renderer);
