all: compile

compile: genclrun gencl.c genclgl.c
	gcc -g -Wall -c gencl.c -o gencl.o
	gcc -g -Wall -c genclgl.c -o genclgl.o
	gcc -g -Wall -c clrun.c -o clrun.o
	gcc -g -Wall --shared dynamiclib.c clrun.o gencl.o genclgl.o -o libclrun.so -ldl
	#	strip libclrun.so

genclrun: ../include/CL/cl.h ../include/CL/cl_gl.h
	./generateClRun.pl ../include/CL/cl.h > gencl.c
	./generateClRun.pl ../include/CL/cl_gl.h > genclgl.c


clean:
	rm -rf *~ *.o
