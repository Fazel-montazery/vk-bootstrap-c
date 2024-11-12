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

State createCommandBuffer(struct Renderer* renderer)
{
	VkCommandBufferAllocateInfo allocInfo = {0};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = renderer->vkCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(renderer->vkDevice, &allocInfo, &renderer->vkCommandBuffer) != VK_SUCCESS) {
		return ERROR_VULKAN_COMMAND_BUFFER_CREATION;
	}
	return SUCCESS;
}

State recordCommandBuffer(struct Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo = {0};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		return ERROR_VULKAN_BEGIN_COMMAND_RECORD;
	}

	VkRenderPassBeginInfo renderPassInfo = {0};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderer->vkRenderPass;
	renderPassInfo.framebuffer = renderer->vkSwapChainFramebuffersVec[imageIndex];
	renderPassInfo.renderArea.offset.x = 0;
	renderPassInfo.renderArea.offset.y = 0;
	renderPassInfo.renderArea.extent = renderer->vkSwapChainExtent;

	VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->vkGraphicsPipeline);

	VkViewport viewport = {0};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) renderer->vkSwapChainExtent.width;
	viewport.height = (float) renderer->vkSwapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor = {0};
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent = renderer->vkSwapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		return ERROR_VULKAN_COMMAND_BUFFER_RECORD;
	}

	return SUCCESS;
}
