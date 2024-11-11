#include "surface.h"

State createSurface(struct Renderer* renderer)
{
	if (glfwCreateWindowSurface(renderer->vkInstance, renderer->window, NULL, &renderer->vkSurface) != VK_SUCCESS) {
		return ERROR_VULKAN_SURFACE_CREATION;
	}
	return SUCCESS;
}
