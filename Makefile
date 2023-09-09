PROGRAM  = app
CXX      = g++
CXXFLAGS = -g -std=c++17 -Wall -Wextra -Werror
INCLUDES = include
SOURCE   = src

$(PROGRAM): main.cpp
	$(CXX) main.cpp $(CXXFLAGS) -o $(PROGRAM) -I $(INCLUDES) -L $(SOURCE) -lglfw -lGLEW -lGL -lGLU -lm

.PHONY: clean

clean:
	-rm -f *.o $(PROGRAM) 
