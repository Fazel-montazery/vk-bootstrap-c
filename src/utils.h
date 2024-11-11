#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint32_t clamp(uint32_t value, uint32_t min, uint32_t max);
unsigned char* loadShaderSpv(const char* path, size_t* outLength);
void unLoadShaderSpv(unsigned char* str);
