GCC = g++

wnv: main.o
	$(GCC) -o wnv main.o


main.o: main.cpp
	$(GCC) -c main.cpp
