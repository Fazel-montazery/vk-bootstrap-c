#pragma once

#include <stdio.h>

typedef enum {
	SUCCESS = 0,
	FAIL,
	ERROR_GLFW_WINDOW_CREATION,
	ERROR_VULKAN_INSTANCE_CREATION,
	ERROR_VULKAN_EXTENSIONS_NOT_SUPPORTED,
	ERROR_VULKAN_VALIDATION_LAYERS_NOT_SUPPORTED,
	ERROR_VULKAN_ERROR_CALLBACK_CREATION,
	ERROR_VULKAN_NO_PHYSICAL_DEVICE_SUPPORTED,
	ERROR_VULKAN_NO_SUITABLE_GPU,
	ERROR_VULKAN_LOGICAL_DEVICE_CREATION,
	ERROR_VULKAN_SURFACE_CREATION,
	ERROR_VULKAN_SWAPCHAIN_CREATION,
	ERROR_VULKAN_IMAGE_VIEW_CREATION
} State;

void printState(State s);

#define CHECK_STATE(state) do {         \
        if (state != SUCCESS) {         \
                printState(state);      \
                return state;           \
        }                               \
} while(0)
