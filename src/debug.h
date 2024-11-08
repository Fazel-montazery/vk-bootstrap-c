#pragma once

#include "config.h"

#include "renderer.h"

#ifndef NDEBUG
State initDebugMessenger(struct Renderer* renderer);
void destroyDebugMessenger(struct Renderer* renderer);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* ci);
#endif
