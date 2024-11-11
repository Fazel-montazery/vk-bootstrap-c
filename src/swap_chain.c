#include "swap_chain.h"

#include "queue.h"

#include "utils.h"

struct SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	struct SwapChainSupportDetails details = {0};

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, NULL);

	if (formatCount != 0) {
		details.formatsVec = vector_create();
		vector_reserve(&details.formatsVec, formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formatsVec);
		vector_get_header(details.formatsVec)->size = formatCount;
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, NULL);

	if (presentModeCount != 0) {
		details.presentModesVec = vector_create();
		vector_reserve(&details.presentModesVec, presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModesVec);
		vector_get_header(details.presentModesVec)->size = presentModeCount;
	}

	return details;
}

void destroySwapChainSupportDetails(struct SwapChainSupportDetails* details)
{
	if (details->formatsVec)
		vector_free(details->formatsVec);

	if (details->presentModesVec)
		vector_free(details->presentModesVec);
}

static VkSurfaceFormatKHR chooseSwapSurfaceFormat(VkSurfaceFormatKHR* availableFormatsVec)
{
	for (int i = 0; i < vector_size(availableFormatsVec); i++) {
		if (availableFormatsVec[i].format == VK_FORMAT_B8G8R8A8_SRGB && availableFormatsVec[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormatsVec[i];
		}
	}

	return availableFormatsVec[0];
}

static VkPresentModeKHR chooseSwapPresentMode(VkPresentModeKHR* availablePresentModesVec) 
{
	for (int i = 0; i < vector_size(availablePresentModesVec); i++) {
		if (availablePresentModesVec[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
			return VK_PRESENT_MODE_MAILBOX_KHR;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D chooseSwapExtent(struct Renderer* renderer, VkSurfaceCapabilitiesKHR capabilities) 
{
	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	} else {
		int width, height;
		glfwGetFramebufferSize(renderer->window, &width, &height);

		VkExtent2D actualExtent = {
			(uint32_t) width,
			(uint32_t) height
		};

		actualExtent.width = clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

State createSwapChain(struct Renderer* renderer)
{
	struct SwapChainSupportDetails swapChainSupport = querySwapChainSupport(renderer->vkPhysicalDevice, renderer->vkSurface);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formatsVec);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModesVec);
	VkExtent2D extent = chooseSwapExtent(renderer, swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {0};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = renderer->vkSurface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	struct QueueFamilyIndices indices = renderer->queueIndices;
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = NULL; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(renderer->vkDevice, &createInfo, NULL, &renderer->vkSwapChain) != VK_SUCCESS) {
		destroySwapChainSupportDetails(&swapChainSupport);
		return ERROR_VULKAN_SWAPCHAIN_CREATION;
	}

	vkGetSwapchainImagesKHR(renderer->vkDevice, renderer->vkSwapChain, &imageCount, NULL);
	renderer->vkSwapChainImagesVec = vector_create();
	vector_reserve(&renderer->vkSwapChainImagesVec, imageCount);
	vkGetSwapchainImagesKHR(renderer->vkDevice, renderer->vkSwapChain, &imageCount, renderer->vkSwapChainImagesVec);
	vector_get_header(renderer->vkSwapChainImagesVec)->size = imageCount;

	renderer->vkSwapChainImageFormat = surfaceFormat.format;
	renderer->vkSwapChainExtent = extent;

	destroySwapChainSupportDetails(&swapChainSupport);
	return SUCCESS;
}

State createImageViews(struct Renderer* renderer)
{
	VkImageView* swapChainImageViewsVec = vector_create();
	int size = vector_size(renderer->vkSwapChainImagesVec);
	vector_reserve(&swapChainImageViewsVec, size);
	vector_get_header(swapChainImageViewsVec)->size = size;

	for (int i = 0; i < size; i++) {
		VkImageViewCreateInfo createInfo = {0};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = renderer->vkSwapChainImagesVec[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = renderer->vkSwapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		if (vkCreateImageView(renderer->vkDevice, &createInfo, NULL, &swapChainImageViewsVec[i]) != VK_SUCCESS) {
			vector_free(swapChainImageViewsVec);
			return ERROR_VULKAN_IMAGE_VIEW_CREATION;
		}
	}

	renderer->vkSwapChainImageViewsVec = swapChainImageViewsVec;

	return SUCCESS;
}
