LFLAGS = -lstdc++ -Wall -fexceptions -g -Iinclude

# comment next line to build client or pass SERVER=1 as argument
# e.g. make -f makefile SERVER=1
# SERVER = 1


ifdef WIN32

WIN_FLAGS = -lws2_32

endif

all: testclient testserver

.PHONY: testclient testserver

ifdef SERVER

testserver: server.o
	g++ $(LFLAGS) server.o $(WIN_FLAGS) -o testserver

server.o: server.cpp
	gcc $(LFLAGS) -c server.cpp

else

testclient: client.o
	g++ $(LFLAGS) client.o $(WIN_FLAGS) -o testclient

client.o: client.cpp
	gcc $(LFLAGS) -c client.cpp

endif