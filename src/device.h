#pragma once

#include "config.h"

#include "renderer.h"

State pickPhysicalDevice(struct Renderer* renderer);
State createLogicalDevice(struct Renderer* renderer);
