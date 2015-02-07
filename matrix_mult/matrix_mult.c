/*  OpenCL implementation of matrix multiplication on two 64 x 64 matrices
	
	The point of this exercise is to develop rudimentary OpenCL programming 
	skills and a familiarity with OCL structures and concepts.
	
	I'll be racing the GPU against the CPU by performing the multiplication 
	on both devices at the same time

*/

#define PROGRAM_NAME "matrix_mult.cl"
#define KERNEL_FUNC "matrix_mult"
#define MATRIX_SIZE 64

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

cl_program build_program(cl_context context, const char* filename) {
	FILE* program_file;
	char* program_buffer;
	size_t program_size;
	int err;

	program_file = fopen(filename, "r");
	if(program_file == NULL) {
		printf("Couldn't find the program file\n");
		exit(1);
	}
	fseek(filename, 0, SEEK_END);
	program_size = ftell(program_file);
	rewind(program_file);

	program_buffer = malloc(program_size + 1);
	program_buffer[program_size] = '\0';

	fread(program_buffer, sizeof(char), program_size, program_file);
	fclose(program_file);

	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer,
		&program_size, &err);
	if(err < 0) {
		printf("Couldn't load program from source\n");
	}

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	return program;
}

cl_device_id create_device(cl_device_type type) {
	cl_platform_id platform;
	cl_device_id device;
	int err;

	err = clGetPlatformIDs(1, &platform, NULL);
	if(err < 0) {
		printf("Couldn't find platform(s)\n");
	}

	err = clGetDeviceIDs(platform, type, 1, &device, NULL);
	if(err < 0) {
		printf("Couldn't access any devices of type %llu\n", type);
	}
	return device;

}

float* make_matrix(size_t nn) {
	float* a = (float*)malloc(nn*sizeof(float));
	for(int i = 0; i < nn; i++) {
		a[i] = rand() / (float)RAND_MAX;
	}
	return a;
}

int main() {
	/*OpenCL structures*/
	cl_device_id devices[2];
	cl_context context;
	cl_program program;
	cl_kernel kernel;
	cl_command_queue cpu_queue, gpu_queue;
	cl_int err;

	/*initialize data*/
	float* a = make_matrix(MATRIX_SIZE);
	float* b = make_matrix(MATRIX_SIZE);

	/*create contexts and devices*/
	devices[0] = create_device(CL_DEVICE_TYPE_CPU);
	devices[1] = create_device(CL_DEVICE_TYPE_GPU);

	context = clCreateContext(NULL, 2, (const cl_device_id*)&devices, NULL, NULL, &err);

	if(err < 0) {
		printf("Couldn't create one or more contexts");
	}

	/*compile program*/
	program = build_program(context, PROGRAM_NAME);

	
	
}








