#include "command.h"

#include "queue.h"

State createCommandPool(struct Renderer* renderer)
{
	struct QueueFamilyIndices queueFamilyIndices = renderer->queueIndices;

	VkCommandPoolCreateInfo poolInfo = {0};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

	if (vkCreateCommandPool(renderer->vkDevice, &poolInfo, NULL, &renderer->vkCommandPool) != VK_SUCCESS) {
		return ERROR_VULKAN_COMMAND_POOL_CREATION;
	}

	return SUCCESS;
}
