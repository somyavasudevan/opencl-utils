all: compile

compile: example1.c example2.cpp
	gcc -Wall example1.c -o example1 -I../../src/clrun -L../../src/clrun -lclrun -ldl
	g++ -g -Wall example2.cpp -o example2 -I../../src/clrun -I../../src/include -L../../src/clrun -lclrun
