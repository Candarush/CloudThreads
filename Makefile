# Имя компилятора
CC=g++

# Стандарт
STD = -std=c++11

# Opencv
OPENCVINCLUDES = $(shell pkg-config --cflags opencv4)
OPENCVLIBS = $(shell pkg-config --libs opencv4)

.PHONY: all clear

all: exec
 
exec: FindCloud.o MosqMQTTWrapper.o main.o 
	$(CC) -o exec FindCloud.o MosqMQTTWrapper.o main.o -lmosquitto -lmosquittopp $(OPENCVLIBS) 
FindCloud.o: FindCloud.hpp FindCloud.cpp
	$(CC) $(STD) $(OPENCVINCLUDES) -c FindCloud.cpp -o FindCloud.o
main.o: main.cpp
	$(CC) $(STD) $(OPENCVINCLUDES) -c main.cpp -o main.o
MosqMQTTWrapper.o: MosqMQTTWrapper.hpp MosqMQTTWrapper.cpp
	$(CC) $(STD) -c MosqMQTTWrapper.cpp -o MosqMQTTWrapper.o
clear:
	rm -rf *.o