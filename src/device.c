#include "device.h"

#include "queue.h"

#include "instance.h"

#ifndef NDEBUG
extern const uint32_t validationLayerCount;
extern const char* validationLayers[];
#endif

static int getDeviceScore(VkPhysicalDevice device) 
{
	struct QueueFamilyIndices indices = findQueueFamilies(device);
	if (!indices.graphicsFamilyExists) {
		return -1;
	}

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
	renderer->queueIndices = findQueueFamilies(tmpDevice);

	vector_free(devicesVec);
	return SUCCESS;
}

State createLogicalDevice(struct Renderer* renderer)
{
	struct QueueFamilyIndices indices = renderer->queueIndices;

	VkDeviceQueueCreateInfo queueCreateInfo = {0};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
	queueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {0};

	VkDeviceCreateInfo createInfo = {0};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = 0;

#ifndef NDEBUG
    		createInfo.enabledLayerCount = validationLayerCount;
    		createInfo.ppEnabledLayerNames = validationLayers;
#else
		createInfo.enabledLayerCount = 0;
#endif

	if (vkCreateDevice(renderer->vkPhysicalDevice, &createInfo, NULL, &renderer->vkDevice) != VK_SUCCESS) {
#ifndef NDEBUG
		fprintf(stderr, "failed to create logical device!\n");
#endif
		return ERROR_VULKAN_LOGICAL_DEVICE_CREATION;
	}

	vkGetDeviceQueue(renderer->vkDevice, indices.graphicsFamily, 0, &renderer->vkGraphicsQueue);
	
	return SUCCESS;
}
