CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: main

main: main.cpp findPathLib.cpp
	$(CXX) $(CXXFLAGS) main.cpp findPathLib.cpp -o main

run: main
	./main