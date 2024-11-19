#include "device.h"

#include "queue.h"

#include "instance.h"
#include "swap_chain.h"

#ifndef NDEBUG
extern const uint32_t validationLayerCount;
extern const char* validationLayers[];
#endif

static const uint32_t deviceExtensionCount = 1;
static char* deviceExtensions[] = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

static bool checkDeviceExtensionSupport(VkPhysicalDevice device, char** requiredDeviceExtensions)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);

	VkExtensionProperties* availableExtensionsVec = vector_create();
	vector_reserve(&availableExtensionsVec, extensionCount);
	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensionsVec);

	for (uint32_t i = 0; i < vector_size(requiredDeviceExtensions); i++) {
		bool found = false;
		for (uint32_t j = 0; j < extensionCount; j++) {
			if (strcmp(requiredDeviceExtensions[i], availableExtensionsVec[j].extensionName) == 0) {
				found = true;
				break;
			}
		}

		if (!found) {
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
#ifndef NDEBUG
			fprintf(stderr, "[Error] (%s) Not supporting (%s) extension!\n", deviceProperties.deviceName, requiredDeviceExtensions[i]);
#endif
			vector_free(availableExtensionsVec);
			return false;
		}
	}

	vector_free(availableExtensionsVec);
	return true;
}

static int getDeviceScore(VkPhysicalDevice device, VkSurfaceKHR surface, char** requiredDeviceExtensions, PhysicalDeviceType preferredType)
{
	struct QueueFamilyIndices indices = findQueueFamilies(device, surface);
	if (!indices.graphicsFamilyExists || !indices.presentFamilyExists) {
		return -1;
	}

	if (!checkDeviceExtensionSupport(device, requiredDeviceExtensions)) {
		return -1;
	}

	bool swapChainAdequate = false;
	struct SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
    	swapChainAdequate = vector_size(swapChainSupport.formatsVec) && vector_size(swapChainSupport.presentModesVec);
	destroySwapChainSupportDetails(&swapChainSupport);

	if (!swapChainAdequate) {
		return -1;
	}

	int score = 0;
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	if (preferredType == PHYSICAL_DEVICE_TYPE_AUTO && deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 10000;
	} else if (deviceProperties.deviceType == (VkPhysicalDeviceType) preferredType) {
		score += 100000;
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
		return ERROR_VULKAN_NO_PHYSICAL_DEVICE_SUPPORTED;
	}

	VkPhysicalDevice* devicesVec = vector_create();
	vector_reserve(&devicesVec, deviceCount);

	vkEnumeratePhysicalDevices(renderer->vkInstance, &deviceCount, devicesVec);

	renderer->vkPhysicalDevice = VK_NULL_HANDLE;

	char** requiredDeviceExtensions = vector_create();
	vector_reserve(&requiredDeviceExtensions, deviceExtensionCount + renderer->config.extraDeviceExtensionsCount);

	for (uint32_t i = 0; i < deviceExtensionCount; i++) {
		vector_add(&requiredDeviceExtensions, deviceExtensions[i]);
	}

	for (uint32_t i = 0; i < renderer->config.extraDeviceExtensionsCount; i++) {
		vector_add(&requiredDeviceExtensions, renderer->config.extraDeviceExtensions[i]);
	}

	int deviceScore = -1;
	VkPhysicalDevice tmpDevice = VK_NULL_HANDLE;
	for (uint32_t i = 0; i < deviceCount; i++) {
		int tmpScore = getDeviceScore(devicesVec[i], renderer->vkSurface, requiredDeviceExtensions, renderer->config.preferredPhysicalDeviceType);
		if (tmpScore > deviceScore) {
			tmpDevice = devicesVec[i];
			deviceScore = tmpScore;
		}
	}

	if (deviceScore == -1) {
		vector_free(devicesVec);
		vector_free(requiredDeviceExtensions);
		return ERROR_VULKAN_NO_SUITABLE_GPU;
	}

	renderer->vkPhysicalDevice = tmpDevice;
	renderer->queueIndices = findQueueFamilies(tmpDevice, renderer->vkSurface);

	renderer->requiredDeviceExtensions = requiredDeviceExtensions;

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

	createInfo.enabledExtensionCount = (uint32_t) vector_size(renderer->requiredDeviceExtensions);
	createInfo.ppEnabledExtensionNames = (const char**) renderer->requiredDeviceExtensions;

#ifndef NDEBUG
    		createInfo.enabledLayerCount = validationLayerCount;
    		createInfo.ppEnabledLayerNames = validationLayers;
#else
		createInfo.enabledLayerCount = 0;
#endif

	if (vkCreateDevice(renderer->vkPhysicalDevice, &createInfo, NULL, &renderer->vkDevice) != VK_SUCCESS) {
		return ERROR_VULKAN_LOGICAL_DEVICE_CREATION;
	}

	vkGetDeviceQueue(renderer->vkDevice, indices.graphicsFamily, 0, &renderer->vkGraphicsQueue);
	vkGetDeviceQueue(renderer->vkDevice, indices.presentFamily, 0, &renderer->vkPresentQueue);
	
	vector_free(renderer->requiredDeviceExtensions);
	vector_free(queueCreateInfosVec);
	vector_free(uniqueQueueFamiliesSet);
	return SUCCESS;
}
