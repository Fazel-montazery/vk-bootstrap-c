#include "instance.h"

#include "debug.h"

#ifndef NDEBUG
extern const uint32_t validationLayerCount;
extern const char* validationLayers[];
#endif

static bool checkExtensionsSupport(const char** requiredExtensionsVec)
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
	VkExtensionProperties* extensionsVec = vector_create();
	vector_reserve(&extensionsVec, extensionCount);
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionsVec);

	for (uint32_t i = 0; i < vector_size(requiredExtensionsVec); i++) {
		bool found = false;
		for (uint32_t j = 0; j < extensionCount; j++) {
			if (strcmp(requiredExtensionsVec[i], extensionsVec[j].extensionName) == 0) {
				found = true;
				break;
			}
		}

		if (!found) {
			vector_free(extensionsVec);
			return false;
		}
	}

	vector_free(extensionsVec);
	return true;
}

#ifndef NDEBUG
static bool checkValidationLayersSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, NULL);

	VkLayerProperties* availableLayersVec = vector_create();
	vector_reserve(&availableLayersVec, layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayersVec);

	for (uint32_t i = 0; i < validationLayerCount; i++) {
		bool found = false;
		for (uint32_t j = 0; j < layerCount; j++) {
			if (strcmp(validationLayers[i], availableLayersVec[j].layerName) == 0) {
				found = true;
				break;
			}
		}

		if (!found) {
			vector_free(availableLayersVec);
			return false;
		}
	}

	vector_free(availableLayersVec);
	return true;
}
#endif

State createVulkanInstance(const char* appName, struct Renderer* renderer)
{
	VkApplicationInfo appInfo = {0};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Fetching and testing extensions
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	uint32_t requiredExtensionCount = glfwExtensionCount;
#if defined __APPLE__ && defined __MACH__
	requiredExtensionCount++;
#endif

#ifndef NDEBUG
	requiredExtensionCount++;
#endif

	const char** requiredExtensionsVec = vector_create();
	vector_reserve(&requiredExtensionsVec, requiredExtensionCount);
	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		vector_add(&requiredExtensionsVec, glfwExtensions[i]);
	}

#if defined __APPLE__ && defined __MACH__
	vector_add(&requiredExtensionsVec, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

#ifndef NDEBUG
	vector_add(&requiredExtensionsVec, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

	if (!checkExtensionsSupport(requiredExtensionsVec)) {
#ifndef NDEBUG
		fprintf(stderr, "[Error] required extensions not supported.\n");
#endif
		vector_free(requiredExtensionsVec);
		return ERROR_VULKAN_EXTENSIONS_NOT_SUPPORTED;
	}

#ifndef NDEBUG
	if (!checkValidationLayersSupport()) {
		fprintf(stderr, "[Error] required validation layers not supported.\n");
		return ERROR_VULKAN_VALIDATION_LAYERS_NOT_SUPPORTED;
	}
#endif

	VkInstanceCreateInfo createInfo = {0};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#if defined __APPLE__ && defined __MACH__
	createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
	createInfo.pApplicationInfo = &appInfo;

#if defined NDEBUG
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = NULL;
#else
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {0};
	createInfo.enabledLayerCount = validationLayerCount;
	createInfo.ppEnabledLayerNames = validationLayers;

	populateDebugMessengerCreateInfo(&debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
#endif

	createInfo.enabledExtensionCount = requiredExtensionCount;
	createInfo.ppEnabledExtensionNames = requiredExtensionsVec;

	if (vkCreateInstance(&createInfo, NULL, &renderer->vkInstance) != VK_SUCCESS) {
#ifndef NDEBUG
		fprintf(stderr, "[Error] Creating vulkan Instance.\n");
#endif
		vector_free(requiredExtensionsVec);
		return ERROR_VULKAN_INSTANCE_CREATION;
	}

	vector_free(requiredExtensionsVec);
	return SUCCESS;
}
