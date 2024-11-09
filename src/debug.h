#pragma once

#ifndef NDEBUG
#include "config.h"

#include "renderer.h"

State initDebugMessenger(struct Renderer* renderer);
void destroyDebugMessenger(struct Renderer* renderer);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* ci);
#endif
