#include <stdio.h>
#include "clrun.h"


int main(int argc, char **argv) {
	int ret;
	if((ret = clrInit()))
		printf("ERROR: %d\n", ret);

	printf("Success! OpenCL %s\n", clrHasOpenCL() ? "exists!" :
		   "does not exist :-(");

	clrExit();
	
	return 0;
}
