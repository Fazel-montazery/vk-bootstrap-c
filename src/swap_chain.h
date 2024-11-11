#pragma once

#include "config.h"

#include "renderer.h"

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	VkSurfaceFormatKHR* formatsVec;
	VkPresentModeKHR* presentModesVec;
};

struct SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
void destroySwapChainSupportDetails(struct SwapChainSupportDetails* details);
State createSwapChain(struct Renderer* renderer);
State createImageViews(struct Renderer* renderer);
