LFLAGS = -lstdc++
VERSION = 4

all: myheap

.PHONY: myheap

myheap: main.o memman_$(VERSION).o
	gcc $(LFLAGS) main.o memman_$(VERSION).o -o myheap

main.o: main.cpp
	gcc $(LFLAGS) -c main.cpp

memman_1.o: memman_1.cpp
	gcc $(LFLAGS) -c memman_1.cpp

memman_2.o: memman_2.cpp
	gcc $(LFLAGS) -c memman_2.cpp

memman_3.o: memman_3.cpp
	gcc $(LFLAGS) -c memman_3.cpp

memman_4.o: memman_4.cpp
	gcc $(LFLAGS) -c memman_4.cpp


