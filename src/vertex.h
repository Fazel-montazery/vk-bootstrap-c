#pragma once

#include "config.h"

#define CGLM_FORCE_DEPTH_ZERO_TO_ONE
#include "../include/cglm/cglm.h"

#include "renderer.h"

struct Vertex2D {
	vec2 pos;
	vec3 color;
};

void initBindingAttribDescriptionVertices();
State createVertexBuffer(struct Renderer* renderer);
