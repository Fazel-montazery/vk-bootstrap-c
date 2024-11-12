#include "sync.h"

State createSyncObjects(struct Renderer* renderer)
{
	VkSemaphoreCreateInfo semaphoreInfo = {0};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {0};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(renderer->vkDevice, &semaphoreInfo, NULL, &renderer->vkImageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(renderer->vkDevice, &semaphoreInfo, NULL, &renderer->vkRenderFinishedSemaphore) != VK_SUCCESS ||
		vkCreateFence(renderer->vkDevice, &fenceInfo, NULL, &renderer->vkInFlightFence) != VK_SUCCESS) {
		return ERROR_VULKAN_SYNC_OBJECTS_CREATION;
	}

	return SUCCESS;
}
