#include "clrun.h"
#include "dynamiclib.h"

int isCL = 0;

// 0 means no opencl, 1 means opencl
int clrInit() {
	int ret = 0;
	
#ifdef _WIN32
	const char *libname = "OpenCL.dll";
#else
	const char *libname = "libOpenCL.so";
#endif

	if((ret = loadLib(libname))) {
		if(ret == -3) // No OpenCL
			return 0;
		else
			return ret;
	}

	isCL = 1;
	
	// TODO: optimize by loading all functions here?

	return 0;
}

int clrHasOpenCL() {
	return isCL;
}

void clrExit() {
	unloadLib();
}

