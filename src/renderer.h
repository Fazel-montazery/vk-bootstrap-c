#pragma once

#include "config.h"

struct Renderer {
	GLFWwindow* window;
	int winWidth;
	int winHeight;
};

State initRenderer(struct Renderer* renderer);
void runRenderer(struct Renderer renderer);
void destroyRenderer(struct Renderer* renderer);
