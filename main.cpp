#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "config.h"
#include "error_handling.h"
#include "index_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

    float vertices[] = {
        -50.0f, -50.0f, 0.0f, 0.0f,  // 0
         50.0f, -50.0f, 1.0f, 0.0f,  // 1
         50.0f,  50.0f, 1.0f, 1.0f,  // 2
        -50.0f,  50.0f, 0.0f, 1.0f,  // 3
    };

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable((GL_BLEND)));

    VertexArray va;
    VertexBuffer vb(vertices, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    va.addBuffer(vb, layout);
    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f, (float) Config::WINDOW_WIDTH, 0.0f, (float) Config::WINDOW_HEIGHT, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    Shader alien_shader("resources/shaders/shader.shader");
    Texture alien_texture("resources/textures/cat.png");
    alien_shader.setUniform1i("u_texture", 0);

    alien_shader.unbind();
    alien_texture.unbind();

    Shader spaceship_shader("resources/shaders/spaceship.shader");
    Texture spaceship_texture("resources/textures/spaceship_fire1.png");
    spaceship_shader.setUniform1i("u_texture", 0);
    
    spaceship_shader.unbind();
    spaceship_texture.unbind();

    va.unbind();
    vb.unbind();
    ib.unbind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    glm::vec3 translationA(200, 200, 0);
    glm::vec3 translationB(400, 200, 0);

    glm::vec3 spaceship_position(500, 100, 0);

    std::vector<glm::vec3> alien_positions = {
        // first row
        glm::vec3(200, 400, 0),
        glm::vec3(400, 400, 0),
        glm::vec3(600, 400, 0),
        glm::vec3(800, 400, 0),
        // second row
        glm::vec3(200, 600, 0),
        glm::vec3(400, 600, 0),
        glm::vec3(600, 600, 0),
        glm::vec3(800, 600, 0)
    };

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render here
        renderer.clear();

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), spaceship_position);
            glm::mat4 mvp = proj * view * model;
            spaceship_shader.bind();
            spaceship_shader.setUniformMat4f("u_modelViewProjectionMatrix", mvp);
            renderer.draw(va, ib, spaceship_shader);
        }

        for (const auto& position : alien_positions) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 mvp = proj * view * model;
            alien_shader.bind();
            alien_shader.setUniformMat4f("u_modelViewProjectionMatrix", mvp);
        
            renderer.draw(va, ib, alien_shader);
        }

        {
            ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 800.0f);
            ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 800.0f);           
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
