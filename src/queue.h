#pragma once

#include "config.h"

struct QueueFamilyIndices {
	uint32_t graphicsFamily;
	bool graphicsFamilyExists;
};

struct QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
