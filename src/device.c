#include "device.h"

static int getDeviceScore(VkPhysicalDevice device) 
{
	int score = 0;
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	score += deviceProperties.limits.maxImageDimension2D;

	return score;
}

State pickPhysicalDevice(struct Renderer* renderer)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(renderer->vkInstance, &deviceCount, NULL);
	if (deviceCount == 0) {
#ifndef NDEBUG
		fprintf(stderr, "failed to find GPUs with Vulkan support!\n");
#endif
		return ERROR_VULKAN_NO_PHYSICAL_DEVICE_SUPPORTED;
	}

	VkPhysicalDevice* devicesVec = vector_create();
	vector_reserve(&devicesVec, deviceCount);

	vkEnumeratePhysicalDevices(renderer->vkInstance, &deviceCount, devicesVec);

	renderer->vkPhysicalDevice = VK_NULL_HANDLE;

	int deviceScore = -1;
	VkPhysicalDevice tmpDevice = VK_NULL_HANDLE;
	for (uint32_t i = 0; i < deviceCount; i++) {
		int tmpScore = getDeviceScore(devicesVec[i]);
		if (tmpScore > deviceScore) {
			tmpDevice = devicesVec[i];
			deviceScore = tmpScore;
		}
	}

	if (deviceScore == -1) {
#ifndef NDEBUG
		fprintf(stderr, "failed to find a suitable GPU!\n");
#endif
		vector_free(devicesVec);
		return ERROR_VULKAN_NO_SUITABLE_GPU;
	}

	renderer->vkPhysicalDevice = tmpDevice;

	vector_free(devicesVec);
	return SUCCESS;
}
