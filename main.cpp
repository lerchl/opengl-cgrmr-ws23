#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct ShaderSources {
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderSources parseShaders(const std::string &filepath) {
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    // 0 = vertex, 1 = fragment
    int type = -1;

    while (getline(stream, line)) {
        if (line.find("#shader") == std::string::npos) {
            ss[(int)type] << line << '\n';
            continue;
        }

        if (line.find("vertex") != std::string::npos) {
            type = 0;
        } else if (line.find("fragment") != std::string::npos) {
            type = 1;
        }
    }

    return {ss[0].str(), ss[1].str()};
}

static unsigned int compileShader(unsigned int type,
                                  const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    // TODO: Error handling
    return id;
}

static unsigned int createShader(ShaderSources shaderSources) {
    unsigned int program = glCreateProgram();
    unsigned int vertexShader =
        compileShader(GL_VERTEX_SHADER, shaderSources.vertexSource);
    unsigned int fragmentShader =
        compileShader(GL_FRAGMENT_SHADER, shaderSources.fragmentSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set GLFW to not create an OpenGL context
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);

    // Create a GLFW windowed mode window and its OpenGL context
    GLFWwindow *window =
        glfwCreateWindow(800, 600, "OpenGL Triangle", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    float vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
    float indices[] = {0, 1, 2, 2, 3, 0};

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * 2 * sizeof(float), indices,
                 GL_STATIC_DRAW);

    unsigned int shader =
        createShader(parseShaders("resources/shaders/orange.shader"));
    glUseProgram(shader);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
