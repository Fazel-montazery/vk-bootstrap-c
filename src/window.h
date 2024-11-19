#pragma once

#include "config.h"

#include "renderer.h"

State initWindow(struct Renderer* renderer);

#ifndef NDEBUG
void updateFpsInWindowTitle(GLFWwindow* win);
#endif
