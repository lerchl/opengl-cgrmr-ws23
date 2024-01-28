#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
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

    // Background
    Shader background_shader("resources/shaders/moon.shader");
    Texture background_texture("resources/textures/moon.png");
    Texture background_normal_map("resources/textures/moon_normal_map.png");
    background_shader.setUniform1i("u_texture", 0);
    background_shader.setUniform1i("u_normalMap", 1);
    background_shader.setUniform1f("u_time", 0.0f);

    // Aliens
    Shader alien_shader("resources/shaders/shader.shader");
    Texture alien_texture("resources/textures/cat.png");
    alien_shader.setUniform1i("u_texture", 0);
    
    // Spaceship
    Shader spaceship_shader("resources/shaders/spaceship.shader");
    Texture spaceship_texture1("resources/textures/spaceship_red1.png");
    Texture spaceship_normal_map("resources/textures/spaceship_red_normal_map.png");
    spaceship_shader.setUniform1i("u_texture", 0);
    spaceship_shader.setUniform1i("u_normalMap", 1); 
    Texture spaceship_texture2("resources/textures/spaceship_red2.png");
    Texture spaceship_texture3("resources/textures/spaceship_red3.png");

    // Bullets
    Shader bullet_shader("resources/shaders/shader.shader");
    Texture bullet_texture("resources/textures/fish.png");
    bullet_shader.setUniform1i("u_texture", 0);

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

    glm::vec3 spaceship_position(500, 100, 0);

    std::vector<glm::vec3> bullet_positions;

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

    // speed
    float spaceship_speed = 600.0f; 
    float bullet_speed = 1000.0f;
    float alien_speed = 400.0f;
    float time = 0.0f;

    double lastFrameTime = glfwGetTime();
    double lastBulletTime = glfwGetTime();
    double spaceship_animation_cycle_time = glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render here
        renderer.clear();

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;

        // Draw background
        {
            background_shader.bind();
            background_texture.bind();
            background_normal_map.bind(1);

            time += 0.25 * deltaTime;
            background_shader.setUniform1f("u_time", time);

            // Draw a quad covering the entire window
            float quadVertices[] = {
                -1.0f, -1.0f, 0.0f, 0.0f,
                 1.0f, -1.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, 1.0f, 1.0f,
                -1.0f,  1.0f, 0.0f, 1.0f
            };

            VertexArray background_va;
            VertexBuffer background_vb(quadVertices, 4 * 4 * sizeof(float));
            VertexBufferLayout background_layout;

            background_layout.push<float>(2);
            background_layout.push<float>(2);
            background_va.addBuffer(background_vb, background_layout);

            IndexBuffer background_ib(indices, 6);

            renderer.draw(background_va, background_ib, background_shader);

            background_texture.unbind();
            background_shader.unbind();
            background_normal_map.unbind();
        }

        // Draw spaceship
        {
            spaceship_shader.bind();
            Texture& spaceship_texture = spaceship_texture1;

            if (currentFrameTime - spaceship_animation_cycle_time < 0.25f) {
                spaceship_texture = spaceship_texture1;
            } else if (currentFrameTime - spaceship_animation_cycle_time > 0.25f && currentFrameTime - spaceship_animation_cycle_time < 0.5f) {
                spaceship_texture = spaceship_texture2;
            } else if (currentFrameTime - spaceship_animation_cycle_time >= 0.5f && currentFrameTime - spaceship_animation_cycle_time < 0.75f) {
                spaceship_texture = spaceship_texture3;
            } else if (currentFrameTime - spaceship_animation_cycle_time >= 0.75f) {
                spaceship_animation_cycle_time = currentFrameTime;
            }

            spaceship_texture.bind();
            spaceship_shader.setUniform1i("u_texture", 0);

            spaceship_normal_map.bind(1);

            // Move spaceship based on key input
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                spaceship_position.x -= spaceship_speed * static_cast<float>(deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                spaceship_position.x += spaceship_speed * static_cast<float>(deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS
                    && currentFrameTime - lastBulletTime > 0.1f
                    && bullet_positions.size() < 3) {
                bullet_positions.push_back(spaceship_position);
                lastBulletTime = currentFrameTime;
            }

            // Keep the spaceship within the window bounds
            if (spaceship_position.x < 50.0f) {
                spaceship_position.x = 50.0f;
            }
            if (spaceship_position.x > Config::WINDOW_WIDTH - 50.0f) {
                spaceship_position.x = Config::WINDOW_WIDTH - 50.0f;
            }

            glm::mat4 model = glm::translate(glm::mat4(1.0f), spaceship_position);
            glm::mat4 mvp = proj * view * model;
            spaceship_shader.setUniformMat4f("u_modelViewProjectionMatrix", mvp);

            renderer.draw(va, ib, spaceship_shader);

            spaceship_texture.unbind();
            spaceship_shader.unbind();
            spaceship_normal_map.unbind();
        }

        // Draw bullets
        {
            bullet_shader.bind();
            bullet_texture.bind();

            for (int i = bullet_positions.size() - 1; i >= 0; i--) {
                auto& position = bullet_positions[i];
                position.y += bullet_speed * static_cast<float>(deltaTime);

                // Check if bullet hit the top window edge
                if (position.y > Config::WINDOW_HEIGHT) {
                    bullet_positions.erase(bullet_positions.begin() + i);
                }

                glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
                glm::mat4 mvp = proj * view * model;
                bullet_shader.setUniformMat4f("u_modelViewProjectionMatrix", mvp);

                renderer.draw(va, ib, bullet_shader);
            }

            bullet_texture.unbind();
            bullet_shader.unbind();
        }

        // Draw aliens
        {
            alien_shader.bind();
            alien_texture.bind();

            for (auto& position : alien_positions) {
                position.x += alien_speed * static_cast<float>(deltaTime);
                 
                // Check if aliens hit the right window edge
                if (position.x > Config::WINDOW_WIDTH - 50.0f) {
                    position.x = Config::WINDOW_WIDTH - 50.0f;
                    alien_speed = -alien_speed; // Reverse direction
                }

                // Check if aliens hit the left window edge
                if (position.x < 50.0f) {
                    position.x = 50.0f;
                    alien_speed = -alien_speed; // Reverse direction
                }
                
                glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
                glm::mat4 mvp = proj * view * model;
                alien_shader.setUniformMat4f("u_modelViewProjectionMatrix", mvp);

                renderer.draw(va, ib, alien_shader);
            }

            alien_texture.unbind();
            alien_shader.unbind();
        }

        // Check if bullet hit an alien
        for (int i = bullet_positions.size() - 1; i >= 0; i--) {
            auto& bullet_position = bullet_positions[i];
            for (int j = alien_positions.size() - 1; j >= 0; j--) {
                auto& alien_position = alien_positions[j];
                if (bullet_position.x > alien_position.x - 50.0f
                        && bullet_position.x < alien_position.x + 50.0f
                        && bullet_position.y > alien_position.y - 50.0f
                        && bullet_position.y < alien_position.y + 50.0f) {
                    bullet_positions.erase(bullet_positions.begin() + i);
                    alien_positions.erase(alien_positions.begin() + j);
                }
            }
        }

        lastFrameTime = currentFrameTime;

        {
            ImGui::SliderFloat3("Spaceship Position", &spaceship_position.x, 0.0f, 800.0f);        
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
