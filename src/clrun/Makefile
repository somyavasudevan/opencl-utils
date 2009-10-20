all: compile

compile: genclrun gencl.c genclgl.c
	gcc -Wall -c gencl.c -o gencl.o
	gcc -Wall -c genclgl.c -o genclgl.o
	gcc -Wall -c dynamiclib.cpp gencl.o genclgl.o -o clrun.o

genclrun: ../include/CL/cl.h ../include/CL/cl_gl.h
	./generateClRun.pl ../include/CL/cl.h > gencl.c
	./generateClRun.pl ../include/CL/cl_gl.h > genclgl.c

