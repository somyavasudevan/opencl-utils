#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <CL/cl.h>
#include "clrun.h"

const char *VectorAddKernel = "                      \
__kernel void AddVectors(const __global float* a,    \
      const __global float* b,                       \
    __global float* c) {                             \
  int ind = get_global_id(0);                        \
  c[ind] = a[ind] + b[ind];                          \
}                                                    \
";
const int count = 26; // Data size


int main(int argc, char **argv) {
    cl_device_id device_id;
    cl_context context;
    cl_command_queue cmdq;
    cl_program program;
    cl_kernel kernel;
    cl_mem A, B, C; // A and B are the input vectors, C is the output
    float *hA = new float[count], *hB = new float[count], *hC = new float[count]; // A, B and C on the main memory

	int ret;
	if((ret = clrInit()))
		printf("ERROR: %d\n", ret);

	printf("Success! OpenCL %s\n", clrHasOpenCL() ? "exists!" :
		   "does not exist :-(");

	if(!clrHasOpenCL())
		return 0;
	
    for(int i = 0; i < count; i++) {  // Filling up A and B with random values
        hA[i] = rand() / (float)RAND_MAX;
        hB[i] = rand() / (float)RAND_MAX;
    }

 
    // OpenCL object initialization
    clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    context = clCreateContext(0, 1, &device_id, NULL, NULL, NULL); 
    cmdq = clCreateCommandQueue(context, device_id, 0, NULL);
    program = clCreateProgramWithSource(context, 1,
                                        (const char **) &VectorAddKernel,
                                        NULL, NULL);

    // Compiling the program and choosing the kernel
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "AddVectors", NULL);

    // Allocating buffers for the vectors
    A = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * count, NULL,
                       NULL);
    B = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * count, NULL,
                       NULL);
    C = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * count, NULL,
                       NULL);
 
    // Copying the input buffers from the main memory to the GPU
    clEnqueueWriteBuffer(cmdq, A, CL_TRUE, 0, sizeof(float) * count, hA, 0, NULL,
                         NULL);
    clEnqueueWriteBuffer(cmdq, B, CL_TRUE, 0, sizeof(float) * count, hB, 0, NULL,
                         NULL);

    // Setting the kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &A);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &B);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &C);

    // Retrieving OpenCL workgroup info
    unsigned int global = count;

    // Enqueue the kernel on the command queue
    clEnqueueNDRangeKernel(cmdq, kernel, 1, NULL, &global, NULL, 0, NULL, NULL);

    // Wait for the kernel to finish
    clFinish(cmdq);

    // Get the results back from the GPU, the results should be in hC
    clEnqueueReadBuffer(cmdq, C, CL_TRUE, 0, sizeof(float) * count, hC, 0, NULL,
                        NULL);

    // NEW: Verify the results
    for(int i = 0; i < count; i++) {
        if(fabs((hA[i] + hB[i]) - hC[i]) > 1e-6) {
            printf("ERROR %d: %f + %f != %f\n", i, hA[i], hB[i], hC[i]);
            return 1;
        }
    }
    printf("Well done! Results are fine!\n");
  
    // We’re done! Release the objects
    clReleaseMemObject(A);
    clReleaseMemObject(B);
    clReleaseMemObject(C);

    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(cmdq);
    clReleaseContext(context);
    delete[] hA;    delete[] hB;    delete[] hC;

    return 0;

}
