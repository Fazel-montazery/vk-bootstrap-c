#include "command.h"

#include "queue.h"

static uint32_t currentFrame = 0; // For frames in flight

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

State createCommandBuffers(struct Renderer* renderer)
{
	VkCommandBufferAllocateInfo allocInfo = {0};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = renderer->vkCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

	if (vkAllocateCommandBuffers(renderer->vkDevice, &allocInfo, renderer->vkCommandBuffers) != VK_SUCCESS) {
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

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		return ERROR_VULKAN_COMMAND_BUFFER_RECORD;
	}

	return SUCCESS;
}

State drawFrame(struct Renderer* r)
{
	vkWaitForFences(r->vkDevice, 1, &r->vkInFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	vkResetFences(r->vkDevice, 1, &r->vkInFlightFences[currentFrame]);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(r->vkDevice, r->vkSwapChain, UINT64_MAX, r->vkImageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	vkResetCommandBuffer(r->vkCommandBuffers[currentFrame], 0);
	recordCommandBuffer(r, r->vkCommandBuffers[currentFrame], imageIndex);

	VkSubmitInfo submitInfo = {0};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { r->vkImageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &r->vkCommandBuffers[currentFrame];

	VkSemaphore signalSemaphores[] = { r->vkRenderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(r->vkGraphicsQueue, 1, &submitInfo, r->vkInFlightFences[currentFrame]) != VK_SUCCESS) {
		return ERROR_VULKAN_DRAW_COMMAND_SUBMIT;
	}

	VkPresentInfoKHR presentInfo = {0};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { r->vkSwapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(r->vkPresentQueue, &presentInfo);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

	return SUCCESS;
}
