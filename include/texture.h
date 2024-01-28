#pragma once

#include <GL/glew.h>

#include <string>

class Texture {
   public:
    Texture(const std::string& filepath);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

   private:
    unsigned int rendererId;
    std::string filepath;
    unsigned char* localBuffer;
    int width;
    int height;
    // bits per pixel
    int bpp;
};
