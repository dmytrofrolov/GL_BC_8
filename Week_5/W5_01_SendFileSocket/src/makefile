#
# Copyright 2015. Dmytro Frolov.  All Rights Reserved.
# 
# Makefile for SendFile project.
#


LFLAGS = -lstdc++ -Wall -fexceptions -g -Iinclude

ifdef WIN32

WIN_FLAGS = -lws2_32

endif

all: SendFileSocket

.PHONY: SendFileSocket

SendFileSocket: main.o CrossPlatformTCPSocket.o Server.o Client.o File.o MainMenu.o
	g++ $(LFLAGS) main.o CrossPlatformTCPSocket.o Server.o Client.o File.o MainMenu.o $(WIN_FLAGS) -o ../bin/SendFileSocket

main.o: main.cpp
	gcc $(LFLAGS) -c main.cpp

CrossPlatformTCPSocket.o: CrossPlatformTCPSocket.cpp
	gcc $(LFLAGS) -c CrossPlatformTCPSocket.cpp

MainMenu.o: MainMenu.cpp
	gcc $(LFLAGS) -c MainMenu.cpp

Server.o: Server.cpp
	gcc $(LFLAGS) -c Server.cpp

Client.o: Client.cpp
	gcc $(LFLAGS) -c Client.cpp


File.o: File.cpp
	gcc $(LFLAGS) -c File.cpp



