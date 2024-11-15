#include "vertex.h"

VkVertexInputBindingDescription bindingDescriptionVertex2D = {0};
VkVertexInputAttributeDescription attributeDescriptionsVertex2D[2] = {0};

void initBindingAttribDescriptionVertices()
{
	// Vertex 2D
	bindingDescriptionVertex2D.binding = 0;
	bindingDescriptionVertex2D.stride = sizeof(struct Vertex2D);
	bindingDescriptionVertex2D.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	attributeDescriptionsVertex2D[0].binding = 0;
	attributeDescriptionsVertex2D[0].location = 0;
	attributeDescriptionsVertex2D[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptionsVertex2D[0].offset = offsetof(struct Vertex2D, pos);

	attributeDescriptionsVertex2D[1].binding = 0;
	attributeDescriptionsVertex2D[1].location = 1;
	attributeDescriptionsVertex2D[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptionsVertex2D[1].offset = offsetof(struct Vertex2D, color);
}

static uint32_t findMemoryType(struct Renderer* renderer, uint32_t typeFilter, VkMemoryPropertyFlags properties) 
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(renderer->vkPhysicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i)) {
			return i;
		}
	}

}

State createVertexBuffer(struct Renderer* renderer)
{
	const struct Vertex2D vertices[] = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	VkBufferCreateInfo bufferInfo = {0};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(vertices);
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(renderer->vkDevice, &bufferInfo, NULL, &renderer->vkVertexBuffer) != VK_SUCCESS) {
		return ERROR_VULKAN_VERTEX_BUFFER_CREATION;
	}

	return SUCCESS;
}
