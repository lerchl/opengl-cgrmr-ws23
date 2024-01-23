PROGRAM    = app
CXX        = g++
CXXFLAGS   = -g -std=c++20 -Wall -Wextra
INCLUDES   = include
LIB        = lib
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

$(PROGRAM): folders config error_handling vertex_buffer index_buffer vertex_array vertex_buffer_layout shader renderer texture lib_stb_image imgui_demo imgui_draw imgui_impl_glfw imgui_impl_opengl3 imgui_tables imgui_widgets imgui main.cpp
	$(CXX) main.cpp $(CXXFLAGS) -o $(PROGRAM) -I $(INCLUDES) -I $(LIB) $(OBJECT_OUT)/*.o -lglfw -lGLEW -lGL -lGLU -lm -ldl

config:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -I $(LIB) -o $(OBJECT_OUT)/config.o -c $(SOURCE)/config.cpp

error_handling:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -I $(LIB) -o $(OBJECT_OUT)/error_handling.o -c $(SOURCE)/error_handling.cpp

vertex_buffer:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -I $(LIB) -o $(OBJECT_OUT)/vertex_buffer.o -c $(SOURCE)/vertex_buffer.cpp

index_buffer:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -I $(LIB) -o $(OBJECT_OUT)/index_buffer.o -c $(SOURCE)/index_buffer.cpp

vertex_array:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -I $(LIB) -o $(OBJECT_OUT)/vertex_array.o -c $(SOURCE)/vertex_array.cpp

vertex_buffer_layout:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -I $(LIB) -o $(OBJECT_OUT)/vertex_buffer_layout.o -c $(SOURCE)/vertex_buffer_layout.cpp

shader:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -I $(LIB) -o $(OBJECT_OUT)/shader.o -c $(SOURCE)/shader.cpp

renderer:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -I $(LIB) -o $(OBJECT_OUT)/renderer.o -c $(SOURCE)/renderer.cpp

texture:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -I $(LIB) -o $(OBJECT_OUT)/texture.o -c $(SOURCE)/texture.cpp

lib_stb_image:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/stb_image.o -c $(SOURCE)/lib_stb_image.cpp

imgui_demo:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/imgui_demo.o -c $(SOURCE)/imgui_demo.cpp

imgui_draw:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/imgui_draw.o -c $(SOURCE)/imgui_draw.cpp

imgui_impl_glfw:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/imgui_impl_glfw.o -c $(SOURCE)/imgui_impl_glfw.cpp

imgui_impl_opengl3:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/imgui_impl_opengl3.o -c $(SOURCE)/imgui_impl_opengl3.cpp

imgui_tables:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/imgui_tables.o -c $(SOURCE)/imgui_tables.cpp

imgui_widgets:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/imgui_widgets.o -c $(SOURCE)/imgui_widgets.cpp

imgui:
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -o $(OBJECT_OUT)/imgui.o -c $(SOURCE)/imgui.cpp
