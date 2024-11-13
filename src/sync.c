#include "sync.h"

State createSyncObjects(struct Renderer* renderer)
{
	VkSemaphoreCreateInfo semaphoreInfo = {0};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {0};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(renderer->vkDevice, &semaphoreInfo, NULL, &renderer->vkImageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(renderer->vkDevice, &semaphoreInfo, NULL, &renderer->vkRenderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(renderer->vkDevice, &fenceInfo, NULL, &renderer->vkInFlightFences[i]) != VK_SUCCESS) {
			return ERROR_VULKAN_SYNC_OBJECTS_CREATION;
		}
	}

	return SUCCESS;
}
