#pragma once

#include "config.h"

#include "renderer.h"

State createCommandPool(struct Renderer* renderer);
State createCommandBuffer(struct Renderer* renderer);
State recordCommandBuffer(struct Renderer* renderer, VkCommandBuffer commandBuffer, uint32_t imageIndex);
