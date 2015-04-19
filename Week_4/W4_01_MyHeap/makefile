LFLAGS = -lstdc++

all: myheap01

myheap01: main.o memman_1.o
	gcc $(LFLAGS) main.o memman_1.o -o myheap01

main.o: main.cpp
	gcc $(LFLAGS) -c main.cpp

memman_1.o: memman_1.cpp
	gcc $(LFLAGS) -c memman_1.cpp


