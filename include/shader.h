#pragma once

#include <unordered_map>
#include <string>

#include "glm/glm.hpp"

struct ShaderSources {
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader {
    public:
        Shader(const std::string& filepath);
        ~Shader();

        void bind() const;
        void unbind() const;

        void setUniform1i(const std::string& name, int value);
        void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
    private:
        unsigned int rendererId;
        std::unordered_map<std::string, int> uniformLocationCache;

        ShaderSources parseShaders(const std::string &filepath);
        unsigned int createShader(ShaderSources shaderSources);
        unsigned int compileShader(unsigned int type, const std::string &source);

        unsigned int getUniformLocation(const std::string& name);
};