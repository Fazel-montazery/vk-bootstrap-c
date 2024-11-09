#include "queue.h"

struct QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
{
	struct QueueFamilyIndices indices = {0};

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

	VkQueueFamilyProperties* queueFamiliesVec = vector_create();
	vector_reserve(&queueFamiliesVec, queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiliesVec);

	for (int i = 0; i < queueFamilyCount; i++) {
		if (indices.graphicsFamilyExists) {
			break;
		}

		if (queueFamiliesVec[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
			indices.graphicsFamilyExists = true;
		}
	}

	vector_free(queueFamiliesVec);
	return indices;
}
