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

	case ERROR_VULKAN_SWAPCHAIN_ACQUIRE_IMAGE:
		fprintf(stderr, "[Error] failed to acquire swap chain image!\n");
		break;

	case ERROR_VULKAN_SWAPCHAIN_PRESENT_IMAGE:
		fprintf(stderr, "[Error] failed to present swap chain image!\n");
		break;

	case ERROR_VULKAN_IMAGE_VIEW_CREATION:
		fprintf(stderr, "[Error] failed to create image views!\n");
		break;

	case ERROR_VULKAN_SHADER_SRC_LOADING:
		fprintf(stderr, "[Error] failed to load shader src!\n");
		break;

	case ERROR_VULKAN_SHADER_MODULE_CREATION:
		fprintf(stderr, "[Error] failed to create shader module!\n");
		break;

	case ERROR_VULKAN_RENDERPASS_CREATION:
		fprintf(stderr, "[Error] failed to create render pass!\n");
		break;

	case ERROR_VULKAN_PIPELINE_LAYOUT_CREATION:
		fprintf(stderr, "[Error] failed to create pipeline layout!\n");
		break;

	case ERROR_VULKAN_GRAPHICS_PIPELINE_CREATION:
		fprintf(stderr, "[Error] failed to create graphics pipeline!\n");
		break;

	case ERROR_VULKAN_FRAME_BUFFER_CREATION:
		fprintf(stderr, "[Error] failed to create framebuffer!\n");
		break;

	case ERROR_VULKAN_COMMAND_POOL_CREATION:
		fprintf(stderr, "[Error] failed to create command pool!\n");
		break;
	
	case ERROR_VULKAN_COMMAND_BUFFER_CREATION:
		fprintf(stderr, "[Error] failed to create command buffer!\n");
		break;

	case ERROR_VULKAN_BEGIN_COMMAND_RECORD:
		fprintf(stderr, "[Error] failed to begin recording command buffer!\n");
		break;

	case ERROR_VULKAN_COMMAND_BUFFER_RECORD:
		fprintf(stderr, "[Error] failed to record command buffer!\n");
		break;

	case ERROR_VULKAN_SYNC_OBJECTS_CREATION:
		fprintf(stderr, "[Error] failed to create semaphores or fences!\n");
		break;

	case ERROR_VULKAN_DRAW_COMMAND_SUBMIT:
		fprintf(stderr, "[Error] failed to submit draw command buffer!\n");
		break;

	default:
		fprintf(stderr, "Unknown state!\n");
	}
}
