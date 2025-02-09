#include "utils.h"

uint32_t clamp(uint32_t value, uint32_t min, uint32_t max)
{
	if (value < min) {
		return min;
	} else if (value > max) {
		return max;
	} else {
		return value;
	}
}

unsigned char* loadShaderSpv(const char* path, size_t* outLength)
{
	if (!path) {
		fprintf(stderr, "[shader src loading] Path is NULL\n");
		return NULL;
	}

	FILE *file = fopen(path, "rb");
	if (!file) {
		fprintf(stderr, "[shader src loading] Failed to open file: [%s]\n", path);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	if (length < 0) {
		fprintf(stderr, "[shader src loading] Failed to determine the length of the file: [%s]\n", path);
		fclose(file);
		return NULL;
	}
	rewind(file);

	unsigned char* buffer = (unsigned char*)malloc(length);
	if (!buffer) {
		fprintf(stderr, "[shader src loading] Failed to allocate memory for ShaderSrc: [%s]\n", path);
		fclose(file);
		return NULL;
	}

	size_t read_length = fread(buffer, 1, length, file);
	if (read_length != length) {
		fprintf(stderr, "[shader src loading] Failed to read the entire file: [%s]\n", path);
		fclose(file);
		free(buffer);
		return NULL;
	}

	fclose(file);
	if (outLength) {
		*outLength = length;
	}
	return buffer;
}

void unLoadShaderSpv(unsigned char* str)
{
	if (str) free(str);
}
