#include "renderer.h"

int main(int argc, char** argv)
{
	struct Renderer myRenderer;
	if (initRenderer(&myRenderer) != SUCCESS)
		return FAIL;

	runRenderer(myRenderer);
	
	destroyRenderer(&myRenderer);
	return SUCCESS;
}
