#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "config.h"
#include "error_handling.h"
#include "index_buffer.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, "CGRMR OPENGL", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    float vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    VertexArray va;
    VertexBuffer vb(vertices, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(2);
    va.addBuffer(vb, layout);
    IndexBuffer ib(indices, 6);

    Shader shader("resources/shaders/shader.shader");
    shader.bind();
    shader.setUniform4f("u_color", 0.2f, 0.3f, 0.8f, 1.0f);
    shader.unbind();

    vb.unbind();
    ib.unbind();

    float r = 0.0f;
    float increment = 0.05f;
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render here
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        shader.bind();
        shader.setUniform4f("u_color", r, 0.3f, 0.8f, 1.0f);

        va.bind();
        ib.bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f) {
            increment = -0.05f;
        } else if (r < 0.0f) {
            increment = 0.05f;
        }

        r += increment;

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
