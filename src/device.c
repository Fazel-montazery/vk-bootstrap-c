#include "device.h"

#include "queue.h"

#include "instance.h"

#ifndef NDEBUG
extern const uint32_t validationLayerCount;
extern const char* validationLayers[];
#endif

static int getDeviceScore(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	struct QueueFamilyIndices indices = findQueueFamilies(device, surface);
	if (!indices.graphicsFamilyExists || !indices.presentFamilyExists) {
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

static bool cmpIndex(const void* a, const void* b)
{
	return *((uint32_t*) a) == *((uint32_t*) b);
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
		int tmpScore = getDeviceScore(devicesVec[i], renderer->vkSurface);
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
	renderer->queueIndices = findQueueFamilies(tmpDevice, renderer->vkSurface);

	vector_free(devicesVec);
	return SUCCESS;
}

State createLogicalDevice(struct Renderer* renderer)
{
	struct QueueFamilyIndices indices = renderer->queueIndices;

	VkDeviceQueueCreateInfo* queueCreateInfosVec = vector_create();
	uint32_t* uniqueQueueFamiliesSet = vector_create();
	vector_add_unique(&uniqueQueueFamiliesSet, indices.graphicsFamily, cmpIndex);
	vector_add_unique(&uniqueQueueFamiliesSet, indices.presentFamily, cmpIndex);

	float queuePriority = 1.0f;
	for (int i = 0; i < vector_size(uniqueQueueFamiliesSet); i++) {
		VkDeviceQueueCreateInfo queueCreateInfo = {0};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = uniqueQueueFamiliesSet[i];
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		vector_add(&queueCreateInfosVec, queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {0};

	VkDeviceCreateInfo createInfo = {0};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = (uint32_t) vector_size(queueCreateInfosVec);
	createInfo.pQueueCreateInfos = queueCreateInfosVec;
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
	vkGetDeviceQueue(renderer->vkDevice, indices.presentFamily, 0, &renderer->vkPresentQueue);
	
	vector_free(queueCreateInfosVec);
	vector_free(uniqueQueueFamiliesSet);
	return SUCCESS;
}
