#include "framebuffer.h"

State createFramebuffers(struct Renderer* renderer)
{
	VkFramebuffer* swapChainFramebuffersVec = vector_create();
	uint32_t size = vector_size(renderer->vkSwapChainImageViewsVec);
	vector_reserve(&swapChainFramebuffersVec, size);
	vector_get_header(swapChainFramebuffersVec)->size = size;

	for (uint32_t i = 0; i < size; i++) {
		VkImageView attachments[] = {
			renderer->vkSwapChainImageViewsVec[i]
		};

		VkFramebufferCreateInfo framebufferInfo = {0};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderer->vkRenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = renderer->vkSwapChainExtent.width;
		framebufferInfo.height = renderer->vkSwapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(renderer->vkDevice, &framebufferInfo, NULL, &swapChainFramebuffersVec[i]) != VK_SUCCESS) {
			return ERROR_VULKAN_FRAME_BUFFER_CREATION;
		}
	}

	renderer->vkSwapChainFramebuffersVec = swapChainFramebuffersVec;
	return SUCCESS;
}
