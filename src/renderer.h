#pragma once

#include "config.h"

#define CHECK_STATE(state) do {         \
        if (state != SUCCESS)           \
                return state;           \
} while(0)

struct Renderer {
	// Windowing
	GLFWwindow* window;
	int winWidth;
	int winHeight;

	// Vulkan handles
	VkInstance vkInstance;
};

State initRenderer(struct Renderer* renderer);
void runRenderer(struct Renderer renderer);
void destroyRenderer(struct Renderer* renderer);
