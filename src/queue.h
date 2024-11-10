#pragma once

#include "config.h"

struct QueueFamilyIndices {
	uint32_t graphicsFamily;
	bool graphicsFamilyExists;
	uint32_t presentFamily;
	bool presentFamilyExists;
};

struct QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
