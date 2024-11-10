#include "queue.h"

struct QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	struct QueueFamilyIndices indices = {0};

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

	VkQueueFamilyProperties* queueFamiliesVec = vector_create();
	vector_reserve(&queueFamiliesVec, queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiliesVec);

	for (int i = 0; i < queueFamilyCount; i++) {
		if (queueFamiliesVec[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
			indices.graphicsFamilyExists = true;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport) {
			indices.presentFamily = i;
			indices.presentFamilyExists = true;
		}

		if (indices.graphicsFamilyExists && indices.presentFamilyExists) {
			break;
		}
	}

	vector_free(queueFamiliesVec);
	return indices;
}
