PROGRAM    = app
CXX        = g++
CXXFLAGS   = -g -std=c++20 -Wall -Wextra
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

$(PROGRAM): folders config error_handling vertex_buffer index_buffer vertex_array vertex_buffer_layout shader main.cpp
	$(CXX) main.cpp $(CXXFLAGS) -o $(PROGRAM) -I $(INCLUDES) $(OBJECT_OUT)/*.o -lglfw -lGLEW -lGL -lGLU -lm

config:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/config.o -c src/config.cpp

error_handling:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/error_handling.o -c src/error_handling.cpp

vertex_buffer:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/vertex_buffer.o -c src/vertex_buffer.cpp

index_buffer:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/index_buffer.o -c src/index_buffer.cpp

vertex_array:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/vertex_array.o -c src/vertex_array.cpp

vertex_buffer_layout:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/vertex_buffer_layout.o -c src/vertex_buffer_layout.cpp

shader:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/shader.o -c src/shader.cpp
