#include "shader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "error_handling.h"

Shader::Shader(const std::string& filepath) : rendererId(createShader(parseShaders(filepath))) {
    // noop
}

Shader::~Shader() {
    GLCall(glDeleteProgram(rendererId));
}

void Shader::bind() const {
    GLCall(glUseProgram(rendererId));
}

void Shader::unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::createShader(ShaderSources shaderSources) {
    unsigned int program = glCreateProgram();
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, shaderSources.vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, shaderSources.fragmentSource);

    GLCall(glAttachShader(program, vertexShader));
    GLCall(glAttachShader(program, fragmentShader));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));

    return program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source) {
    GLCall(unsigned int id = glCreateShader(type));
    const char *src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, NULL));
    GLCall(glCompileShader(id));
    // TODO: Error handling
    return id;
}

ShaderSources Shader::parseShaders(const std::string &filepath) {
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

unsigned int Shader::getUniformLocation(const std::string& name) {
    if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
        return uniformLocationCache[name];
    }

    GLCall(int location = glGetUniformLocation(rendererId, name.c_str()));
    uniformLocationCache[name] = location;

    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }

    return location;
}
