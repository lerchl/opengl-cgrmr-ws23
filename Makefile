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

$(PROGRAM): folders config error_handling vertex_buffer index_buffer vertex_array vertex_buffer_layout shader renderer texture lib_stb_image main.cpp
	$(CXX) main.cpp $(CXXFLAGS) -o $(PROGRAM) -I $(INCLUDES) $(OBJECT_OUT)/*.o -lglfw -lGLEW -lGL -lGLU -lm

config:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/config.o -c $(SOURCE)/config.cpp

error_handling:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/error_handling.o -c $(SOURCE)/error_handling.cpp

vertex_buffer:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/vertex_buffer.o -c $(SOURCE)/vertex_buffer.cpp

index_buffer:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/index_buffer.o -c $(SOURCE)/index_buffer.cpp

vertex_array:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/vertex_array.o -c $(SOURCE)/vertex_array.cpp

vertex_buffer_layout:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/vertex_buffer_layout.o -c $(SOURCE)/vertex_buffer_layout.cpp

shader:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/shader.o -c $(SOURCE)/shader.cpp

renderer:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/renderer.o -c $(SOURCE)/renderer.cpp

texture:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/texture.o -c $(SOURCE)/texture.cpp

lib_stb_image:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/stb_image.o -c $(SOURCE)/lib_stb_image.cpp
