#include "renderer.h"

int main(int argc, char** argv)
{
	struct Renderer myRenderer;
	if (initRenderer(&myRenderer) != SUCCESS)
		return -1;

	runRenderer(myRenderer);
	
	destroyRenderer(&myRenderer);
	return 0;
}
