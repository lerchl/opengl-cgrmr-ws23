#include "texture.h"

#include "lib_stb_image.h"

#include "error_handling.h"

Texture::Texture(const std::string& filepath) : filepath(filepath) {
    stbi_set_flip_vertically_on_load(1);
    localBuffer = stbi_load(filepath.c_str(), &width, &height, &bpp, 4);

    GLCall(glGenTextures(1, &rendererId));
    bind();

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
    unbind();

    if (localBuffer) {
        stbi_image_free(localBuffer);
    }
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &rendererId));
}

void Texture::bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, rendererId));
}

void Texture::unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
