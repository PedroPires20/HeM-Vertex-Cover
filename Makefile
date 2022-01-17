CXX := g++
CXX_FLAGS := -std=c++17 -Wall
EXEC_NAME := MVCSolver

all: main

clean:
	rm -rf bin/*.o

main: bin/graph.o bin/main.o
	${CXX} ${CXX_FLAGS} $^ -o bin/${EXEC_NAME}

bin/main.o: src/main.cpp
	${CXX} ${CXX_FLAGS} $^ -c -o $@

bin/graph.o: src/graph.cpp
	${CXX} ${CXX_FLAGS} $^ -c -o $@

