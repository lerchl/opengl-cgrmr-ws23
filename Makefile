PROGRAM    = app
CXX        = g++
CXXFLAGS   = -g -std=c++17 -Wall -Wextra -Werror
INCLUDES   = include
SOURCE     = src
OUT        = build
OBJECT_OUT = $(OUT)/object

default: $(PROGRAM)

run: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -r $(OUT)

.PHONY: clean

folders:
	mkdir -p $(OBJECT_OUT)

$(PROGRAM): folders config main.cpp
	$(CXX) main.cpp $(CXXFLAGS) -o $(PROGRAM) -I $(INCLUDES) $(OBJECT_OUT)/*.o -lglfw -lGLEW -lGL -lGLU -lm

config:
	$(CXX) $(CXXFLAGS) -o $(OBJECT_OUT)/config.o -c src/config.cpp

