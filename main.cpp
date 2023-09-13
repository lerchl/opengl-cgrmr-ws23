#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>

#include "include/config.h"

void glfwErrorCallback(const int error, const char* description) {
    fprintf(stderr, "Error: %d %s\n", error, description);
}

int main(void) {
    // Set the error callback for GLFW
    glfwSetErrorCallback(glfwErrorCallback);

    // Initialize GLFW
    if (glfwInit() == GLFW_FALSE) {
        // GLFW failed!
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, "My Title", NULL, NULL);
    if (!window) {
        // Window or context creation failed
        std::cerr << "Window or context creation failed!" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    GLenum glewInitStatus = glewInit();
    if (glewInitStatus != GLEW_OK) {
        // GLEW failed!
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glewInitStatus));
        glfwTerminate();
        return -1;
    }

    // GLEW initialized successfully!
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    int counter = 0;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(counter / 1000.0, counter / 1000.0, counter / 1000.0, 1.0);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();

        counter = (counter + 1) % 1000;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
