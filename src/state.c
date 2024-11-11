#include "state.h"

void printState(State s)
{
	switch (s) {
	case SUCCESS:
		fprintf(stderr, "SUCCESS!\n");
		break;

	case FAIL:
		fprintf(stderr, "FAIL!\n");
		break;
		
	case ERROR_GLFW_WINDOW_CREATION:
		fprintf(stderr, "[Error] creating glfw window!\n");
		break;

	case ERROR_VULKAN_INSTANCE_CREATION:
		fprintf(stderr, "[Error] Creating vulkan Instance!\n");
		break;

	case ERROR_VULKAN_EXTENSIONS_NOT_SUPPORTED:
		fprintf(stderr, "[Error] required extensions not supported!\n");
		break;

	case ERROR_VULKAN_VALIDATION_LAYERS_NOT_SUPPORTED:
		fprintf(stderr, "[Error] required validation layers not supported!\n");
		break;

	case ERROR_VULKAN_ERROR_CALLBACK_CREATION:
		fprintf(stderr, "[Error] failed to set up debug messenger!\n");
		break;

	case ERROR_VULKAN_NO_PHYSICAL_DEVICE_SUPPORTED:
		fprintf(stderr, "[Error] failed to find GPUs with Vulkan support!\n");
		break;

	case ERROR_VULKAN_NO_SUITABLE_GPU:
		fprintf(stderr, "[Error] failed to find a suitable GPU!\n");
		break;

	case ERROR_VULKAN_LOGICAL_DEVICE_CREATION:
		fprintf(stderr, "[Error] failed to create logical device!\n");
		break;

	case ERROR_VULKAN_SURFACE_CREATION:
		fprintf(stderr, "[Error] failed to create window surface!\n");
		break;

	case ERROR_VULKAN_SWAPCHAIN_CREATION:
		fprintf(stderr, "[Error] failed to create swap chain!\n");
		break;

	case ERROR_VULKAN_IMAGE_VIEW_CREATION:
		fprintf(stderr, "[Error] failed to create image views!\n");
		break;

	default:
		fprintf(stderr, "Unknown state!\n");
	}
}
